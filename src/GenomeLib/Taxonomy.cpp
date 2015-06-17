#include "Environment.h"
#include "GeneHunterException.h"
#include "Taxonomy.h"
#include <boost/lexical_cast.hpp>
#include <iostream>

using namespace std;

namespace GeneHunter {

Taxonomy::Taxonomy( Settings const& settings )
 : settings_(settings),
   myConnection_(mysql_init(NULL))
{

    if ( mysql_real_connect(myConnection_,
    	Environment::getMysqlHost().c_str(),
    	Environment::getMysqlUser().c_str(),
    	Environment::getMysqlPassword().c_str(),
    	Environment::getMysqlTaxDatabase().c_str(),0,0,0) == NULL )
    {
        throw GeneHunterException(mysql_error(myConnection_));
    }
}

Taxonomy::~Taxonomy()
{
    mysql_close(myConnection_);
}

size_t Taxonomy::getTaxIDByNucGI( size_t geneID ) const
{
	string query = "SELECT tax_id FROM gi_taxid_nuc WHERE gi = " + boost::lexical_cast<string>(geneID);
	if ( mysql_query(myConnection_,query.c_str()) )
		throw GeneHunterException(mysql_error(myConnection_));

	MYSQL_RES *result = mysql_store_result(myConnection_);
	if ( result == NULL )
		throw GeneHunterException(mysql_error(myConnection_));

	MYSQL_ROW row = mysql_fetch_row(result);
	if ( row == NULL )
		throw GeneHunterException("mysql_fetch_row == NULL");

	size_t taxID = boost::lexical_cast<size_t>(row[0]);

	mysql_free_result(result);

	return taxID;
}

std::tuple<size_t,std::string,bool> Taxonomy::getTaxInfo( size_t taxID ) const
{
	string query = "SELECT parent_tax_id, rank, GenBank_hidden_flag FROM ncbi_nodes WHERE tax_id = " + boost::lexical_cast<string>(taxID);
	if ( mysql_query(myConnection_,query.c_str()) )
		throw GeneHunterException(mysql_error(myConnection_));

	MYSQL_RES *result = mysql_store_result(myConnection_);
	if ( result == NULL )
		throw GeneHunterException(mysql_error(myConnection_));

	MYSQL_ROW row = mysql_fetch_row(result);
	if ( row == NULL )
		throw GeneHunterException("mysql_fetch_row == NULL");

	std::tuple<size_t,std::string,bool> taxInfo(boost::lexical_cast<size_t>(row[0]),row[1],boost::lexical_cast<bool>(row[2]));

	mysql_free_result(result);

	return taxInfo;
}

std::string Taxonomy::getLineage( size_t taxID, bool withRank, bool abbreviated ) const
{
	updateLineageTrace(taxID);

	string lineage;
	for ( auto iter = currentTrace_.crbegin(); iter != currentTrace_.crend(); ++iter ) {
		if ( abbreviated and get<3>(*iter) ) continue;
		lineage += get<0>(*iter);
		if ( withRank ) lineage += " (" + get<1>(*iter) + ", " + boost::lexical_cast<string>(get<2>(*iter)) + ")";
		lineage += "; ";
	}

	return lineage.erase(lineage.size()-2) + ".";
}

std::string Taxonomy::getScientificName( size_t taxID, std::string const& rank ) const
{
	if ( rank.empty() ) return getScientificNameByTaxID(taxID);
	if ( rank == "species" ) return getScientificNameOfSpecies(taxID);

	while ( taxID != 1 ) {
		auto taxInfo = getTaxInfo(taxID);
		if ( get<1>(taxInfo) == rank ) return getScientificNameByTaxID(taxID);
		taxID = get<0>(taxInfo);
	}

	return "";
}

std::string Taxonomy::getScientificNameOfSpecies( size_t taxID ) const
{
	string genus, species;

	while ( taxID != 1 ) {
		auto taxInfo = getTaxInfo(taxID);
		if ( get<1>(taxInfo) == "species" ) species = getScientificNameByTaxID(taxID);
		if ( get<1>(taxInfo) == "genus" ) genus = getScientificNameByTaxID(taxID);
		taxID = get<0>(taxInfo);
	}

	if ( !genus.empty() and !species.empty() ) {
		if ( species.substr(0,genus.size()) == genus ) return species.substr(genus.size()+1,species.size()-genus.size());
	}

	return species;
}

size_t Taxonomy::getLowestTaxIDWithRank( size_t taxID ) const
{
	while ( taxID != 1 ) {
		auto taxInfo = getTaxInfo(taxID);
		if ( get<1>(taxInfo) != "no rank" and get<1>(taxInfo) != "subspecies" ) return taxID;
		taxID = get<0>(taxInfo);
	}
	throw GeneHunterException("Taxonomy::getLowestTaxIDWithRank: no lowestTaxIDWithRank found.");
}

std::string Taxonomy::getOrganismName( size_t taxID ) const
{
	string genus = getScientificName(taxID,"genus");
	string species = getScientificName(taxID,"species");

	if ( !genus.empty() and !species.empty() ) return genus + " " + species;
	if ( !genus.empty() ) return genus;

	string speciesGroup = getScientificName(taxID,"species group");

	if ( !speciesGroup.empty() ) return speciesGroup;

	return string("no name found (") + boost::lexical_cast<string>(taxID) + ")";
}

Organism Taxonomy::getOrganism( size_t geneID ) const
{
	size_t taxID = getTaxIDByNucGI(geneID);
	return Organism( getLowestTaxIDWithRank(taxID), getLineage(taxID), getOrganismName(taxID), getScientificName(taxID,"genus"), getScientificName(taxID,"species") );
}

void Taxonomy::updateLineageTrace( size_t taxID ) const
{
	if ( currentTraceTaxID_ == taxID ) return;

	size_t curTaxID = taxID;
	currentTrace_.clear();

	while ( curTaxID != 1 ) {
		auto taxInfo = getTaxInfo(curTaxID);
		currentTrace_.push_back( make_tuple(getScientificNameByTaxID(curTaxID), get<1>(taxInfo), curTaxID, get<2>(taxInfo)) );
		curTaxID = get<0>(taxInfo);
	}

	if ( currentTrace_.empty() ) throw GeneHunterException("Lineage trace is empty.");

	currentTraceTaxID_ = taxID;
}

std::string Taxonomy::getScientificNameByTaxID( size_t taxID ) const
{
	string query = "SELECT name_txt FROM ncbi_names WHERE tax_id = " + boost::lexical_cast<string>(taxID) + " AND name_class = 'scientific name'";
	if ( mysql_query(myConnection_,query.c_str()) )
		throw GeneHunterException(mysql_error(myConnection_));

	MYSQL_RES *result = mysql_store_result(myConnection_);
	if ( result == NULL )
		throw GeneHunterException(mysql_error(myConnection_));

	MYSQL_ROW row = mysql_fetch_row(result);
	if ( row == NULL )
		throw GeneHunterException("mysql_fetch_row == NULL");

	string scientificName(row[0]);

	mysql_free_result(result);

	return scientificName;
}

} // namespace GeneHunter
