#include "TaxonomyWriter.h"
#include "Environment.h"
#include "GeneAssemblerException.h"
#include "Location.h"
#include <boost/algorithm/string/trim.hpp>
#include <boost/tokenizer.hpp>
#include <fstream>
#include <sstream>

using namespace std;
using boost::filesystem::path;

namespace GeneAssembler {

const std::string TaxonomyWriter::giTaxIDNucTableName_ = "gi_taxid_nuc";

const std::string TaxonomyWriter::ncbiNodesTableName_ = "ncbi_nodes";

const std::string TaxonomyWriter::ncbiNamesTableName_ = "ncbi_names";

TaxonomyWriter::TaxonomyWriter( boost::filesystem::path const& gi_taxid_nucl_file, boost::filesystem::path const& names_file,
    boost::filesystem::path const& nodes_file, std::string const& database )
 : myConnection_(mysql_init(NULL))
{
    if ( mysql_real_connect(myConnection_,
    	Environment::getMysqlHost().c_str(),
    	Environment::getMysqlUser().c_str(),
    	Environment::getMysqlPassword().c_str(),
    	database.c_str(),0,0,0) == NULL )
    {
        throw GeneAssemblerException(mysql_error(myConnection_));
    }

    string query = "START TRANSACTION";

	if ( mysql_query(myConnection_,query.c_str()) )
		throw GeneAssemblerException(mysql_error(myConnection_));

	query = "CREATE TABLE IF NOT EXISTS " + giTaxIDNucTableName_ + "(\
			        gi INT PRIMARY KEY,\
			        tax_id INT)\
			        ENGINE = MYISAM";

	if ( mysql_query(myConnection_,query.c_str()) )
		throw GeneAssemblerException(mysql_error(myConnection_));

	query = "CREATE TABLE IF NOT EXISTS " + ncbiNamesTableName_ + "(\
			        tax_id INT,\
			        name_txt VARCHAR(255),\
			        unique_name VARCHAR(255),\
			        name_class VARCHAR(32))\
			        ENGINE = MYISAM";

	if ( mysql_query(myConnection_,query.c_str()) )
		throw GeneAssemblerException(mysql_error(myConnection_));

	query = "CREATE TABLE IF NOT EXISTS " + ncbiNodesTableName_ + "(\
			        tax_id mediumint(11) PRIMARY KEY,\
			        parent_tax_id mediumint(8),\
			        rank varchar(32),\
			        embl_code varchar(16),\
			        division_id smallint(6),\
			        inherited_div_flag tinyint(4),\
			        genetic_code_id smallint(6),\
			        inherited_GC_flag tinyint(4),\
			        mitochondrial_genetic_code_id smallint(4),\
			        inherited_MGC_flag tinyint(4),\
			        GenBank_hidden_flag smallint(4),\
			        hidden_subtree_root_flag tinyint(4),\
			        comments varchar(255))\
			        ENGINE = MYISAM";

	if ( mysql_query(myConnection_,query.c_str()) )
		throw GeneAssemblerException(mysql_error(myConnection_));

	importGITaxIDNuc(gi_taxid_nucl_file);
	importNCBINames(names_file);
	importNCBINodes(nodes_file);
	createIndices();
}

TaxonomyWriter::~TaxonomyWriter()
{
	string query("COMMIT");

	if ( mysql_query(myConnection_,query.c_str()) )
		throw GeneAssemblerException(mysql_error(myConnection_));

    mysql_close(myConnection_);
}

void TaxonomyWriter::importGITaxIDNuc( path const& filename )
{
	ifstream ifs(filename.string().c_str());
	size_t gi,tax_id;
	std::string line;
	while ( std::getline(ifs,line) )
	{
        stringstream ss(line);
        ss >> gi >> tax_id;

		string query = string("INSERT INTO " + giTaxIDNucTableName_ + " SET")
					 + " gi='" + boost::lexical_cast<string>(gi) + "',"
					 + " tax_id='" + boost::lexical_cast<string>(tax_id) + "'";

		if ( mysql_query(myConnection_,query.c_str()) )
			throw GeneAssemblerException(mysql_error(myConnection_));
	}
}

void TaxonomyWriter::importNCBINames( path const& filename )
{
	typedef boost::tokenizer<boost::char_separator<char> > tokenizer;

	ifstream ifs(filename.string().c_str());
	std::string line;
	while ( std::getline(ifs,line) )
	{
	    boost::char_separator<char> sep("|");
		tokenizer tokens(line,sep);
		tokenizer::const_iterator iterCur = tokens.begin();

		std::string tax_id = boost::algorithm::trim_copy(*iterCur); iterCur++;
		std::string name_txt = boost::algorithm::trim_copy(*iterCur); iterCur++;
		std::string unique_name = boost::algorithm::trim_copy(*iterCur); iterCur++;
		std::string name_class = boost::algorithm::trim_copy(*iterCur);

		string query = string("INSERT INTO " + ncbiNamesTableName_ + " SET")
					 + " tax_id='" + strip(tax_id) + "',"
					 + " name_txt='" + strip(name_txt) + "',"
					 + " unique_name='" + strip(unique_name) + "',"
					 + " name_class='" + strip(name_class) + "'";

		if ( mysql_query(myConnection_,query.c_str()) )
			throw GeneAssemblerException(mysql_error(myConnection_));
	}
}

void TaxonomyWriter::importNCBINodes( path const& filename )
{
	typedef boost::tokenizer<boost::char_separator<char> > tokenizer;

	ifstream ifs(filename.string().c_str());
	std::string line;
	while ( std::getline(ifs,line) )
	{
	    boost::char_separator<char> sep("|");
		tokenizer tokens(line,sep);
		tokenizer::const_iterator iterCur = tokens.begin();

		std::string tax_id = boost::algorithm::trim_copy(*iterCur); iterCur++;
		std::string parent_tax_id = boost::algorithm::trim_copy(*iterCur); iterCur++;
		std::string rank = boost::algorithm::trim_copy(*iterCur); iterCur++;
		std::string embl_code = boost::algorithm::trim_copy(*iterCur); iterCur++;
		std::string division_id = boost::algorithm::trim_copy(*iterCur); iterCur++;
		std::string inherited_div_flag = boost::algorithm::trim_copy(*iterCur); iterCur++;
		std::string genetic_code_id = boost::algorithm::trim_copy(*iterCur); iterCur++;
		std::string inherited_GC_flag = boost::algorithm::trim_copy(*iterCur); iterCur++;
		std::string mitochondrial_genetic_code_id = boost::algorithm::trim_copy(*iterCur); iterCur++;
		std::string inherited_MGC_flag = boost::algorithm::trim_copy(*iterCur); iterCur++;
		std::string GenBank_hidden_flag = boost::algorithm::trim_copy(*iterCur); iterCur++;
		std::string hidden_subtree_root_flag = boost::algorithm::trim_copy(*iterCur); iterCur++;
		std::string comments = boost::algorithm::trim_copy(*iterCur);

		string query = string("INSERT INTO " + ncbiNodesTableName_ + " SET")
					 + " tax_id='" + strip(tax_id) + "',"
					 + " parent_tax_id='" + strip(parent_tax_id) + "',"
					 + " rank='" + strip(rank) + "',"
					 + " embl_code='" + strip(embl_code) + "',"
					 + " division_id='" + strip(division_id) + "',"
					 + " inherited_div_flag='" + strip(inherited_div_flag) + "',"
					 + " genetic_code_id='" + strip(genetic_code_id) + "',"
					 + " inherited_GC_flag='" + strip(inherited_GC_flag) + "',"
					 + " mitochondrial_genetic_code_id='" + strip(mitochondrial_genetic_code_id) + "',"
					 + " inherited_MGC_flag='" + strip(inherited_MGC_flag) + "',"
					 + " GenBank_hidden_flag='" + strip(GenBank_hidden_flag) + "',"
					 + " hidden_subtree_root_flag='" + strip(hidden_subtree_root_flag) + "',"
					 + " comments='" + strip(comments) + "'";

		if ( mysql_query(myConnection_,query.c_str()) )
			throw GeneAssemblerException(mysql_error(myConnection_));
	}
}

void TaxonomyWriter::createIndices() const
{
	string query = "CREATE INDEX tax_id ON " + giTaxIDNucTableName_ + " (tax_id)";

	if ( mysql_query(myConnection_,query.c_str()) )
	throw GeneAssemblerException(mysql_error(myConnection_));

	query = "CREATE INDEX tax_id ON " + ncbiNamesTableName_ + " (tax_id)";

	if ( mysql_query(myConnection_,query.c_str()) )
	throw GeneAssemblerException(mysql_error(myConnection_));

	query = "CREATE INDEX name_class ON " + ncbiNamesTableName_ + " (name_class)";

	if ( mysql_query(myConnection_,query.c_str()) )
	throw GeneAssemblerException(mysql_error(myConnection_));

	query = "CREATE INDEX name_txt ON " + ncbiNamesTableName_ + " (name_txt)";

	if ( mysql_query(myConnection_,query.c_str()) )
	throw GeneAssemblerException(mysql_error(myConnection_));

	query = "CREATE INDEX parent_tax_id ON " + ncbiNodesTableName_ + " (parent_tax_id)";

	if ( mysql_query(myConnection_,query.c_str()) )
	throw GeneAssemblerException(mysql_error(myConnection_));
}

string TaxonomyWriter::strip( string const& s ) const
{
	if (s.length() > stripBufferLength_)
		throw GeneAssemblerException("CDSDatabase::strip: buffer overflow.");
	mysql_real_escape_string(myConnection_, stripBuffer_, s.c_str(), s.length());
	return stripBuffer_;
}

} // namespace GeneAssembler
