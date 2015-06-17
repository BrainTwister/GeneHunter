#include "CDSDatabase.h"
#include "Environment.h"
#include "GeneHunterException.h"
#include "Location.h"

using namespace std;

namespace GeneHunter {

CDSDatabase::CDSDatabase( Settings const& settings )
 : settings_(settings),
   myConnection_(mysql_init(NULL))
{
    if ( mysql_real_connect(myConnection_,
    	Environment::getMysqlHost().c_str(),
    	Environment::getMysqlUser().c_str(),
    	Environment::getMysqlPassword().c_str(),
    	Environment::getMysqlMyDatabase().c_str(),0,0,0) == NULL )
    {
        throw GeneHunterException(mysql_error(myConnection_));
    }

	string query = "CREATE TABLE IF NOT EXISTS " + settings_.proteinLinkTableName_ + "(\
			        id INT AUTO_INCREMENT PRIMARY KEY,\
			        geneID INT,\
			        referenceName VARCHAR(200),\
			        location VARCHAR(10000),\
			        enclosingLocationLowerBoundary INT,\
			        enclosingLocationUpperBoundary INT,\
			        enclosingLocationOpenLowerBoundary BOOL,\
			        enclosingLocationOpenUpperBoundary BOOL,\
			        gene VARCHAR(200),\
	                locusTag VARCHAR(200),\
			        proteinID VARCHAR(200),\
	                product VARCHAR(200))\
			        ENGINE = MYISAM";

	if ( mysql_query(myConnection_,query.c_str()) )
		throw GeneHunterException(mysql_error(myConnection_));

	query = "START TRANSACTION";

	if ( mysql_query(myConnection_,query.c_str()) )
		throw GeneHunterException(mysql_error(myConnection_));

	if ( settings_.verbosity_ > 0 ) {
	    cout << "Number of entries in CDSDatabase = " << nbGeneEntries() << endl;
	    cout << "Number of proteinLinks in CDSDatabase = " << nbProteinLinks() << endl;
	}
}

CDSDatabase::~CDSDatabase()
{
	string query("COMMIT");

	if ( mysql_query(myConnection_,query.c_str()) )
		throw GeneHunterException(mysql_error(myConnection_));

    mysql_close(myConnection_);
}

void CDSDatabase::importGene( CDSEntry const& entry )
{
	Range<uint32_t> enclosingRange;
	try {
	    enclosingRange = Location(entry.location_).getEnclosingRange();
	} catch ( ... ) {
		cerr << "Error reading location " << entry.location_ << " in gene " << entry.geneID_ << ". CDS will ignored." << endl;
	}

	if ( entry.location_.size() > 10000 )
		throw GeneHunterException("CDSDatabase::importGene: location string too large.");

	string query = string("INSERT INTO " + settings_.proteinLinkTableName_ + " SET")
		  	     + " geneID='" + boost::lexical_cast<string>(entry.geneID_) + "',"
		  	     + " referenceName='" + strip(entry.referenceName_) + "',"
		  	     + " location='" + strip(entry.location_) + "',"
		  	     + " enclosingLocationLowerBoundary='" + boost::lexical_cast<string>(enclosingRange.getLowerBoundary()) + "',"
			     + " enclosingLocationUpperBoundary='" + boost::lexical_cast<string>(enclosingRange.getUpperBoundary()) + "',"
			     + " enclosingLocationOpenLowerBoundary='" + boost::lexical_cast<string>(enclosingRange.getOpenLowerBoundary()) + "',"
			     + " enclosingLocationOpenUpperBoundary='" + boost::lexical_cast<string>(enclosingRange.getOpenUpperBoundary()) + "',"
			     + " gene='" + strip(entry.ptrProteinLink_->gene_) + "',"
			     + " locusTag='" + strip(entry.ptrProteinLink_->locusTag_) + "',"
			     + " proteinID='" + strip(entry.ptrProteinLink_->proteinID_) + "',"
			     + " product='" + strip(entry.ptrProteinLink_->product_) + "'";

	if ( mysql_query(myConnection_,query.c_str()) )
		throw GeneHunterException(mysql_error(myConnection_));
}

size_t CDSDatabase::nbProteinLinks() const
{
	string query = "SELECT id FROM " + settings_.proteinLinkTableName_ + " ORDER BY id DESC LIMIT 1";
	if ( mysql_query(myConnection_,query.c_str()) )
		throw GeneHunterException(mysql_error(myConnection_));

	MYSQL_RES *result = mysql_store_result(myConnection_);
	if ( result == NULL )
		throw GeneHunterException(mysql_error(myConnection_));

	MYSQL_ROW row = mysql_fetch_row(result);
	if ( row == NULL ) {
		mysql_free_result(result);
		return 0;
	}

	size_t id = boost::lexical_cast<size_t>(row[0]);

	mysql_free_result(result);

	return id;
}

size_t CDSDatabase::nbGeneEntries() const
{
	string query = "SELECT COUNT(DISTINCT geneID) FROM " + settings_.proteinLinkTableName_;
	if ( mysql_query(myConnection_,query.c_str()) )
		throw GeneHunterException(mysql_error(myConnection_));

	MYSQL_RES *result = mysql_store_result(myConnection_);
	if ( result == NULL )
		throw GeneHunterException(mysql_error(myConnection_));

	MYSQL_ROW row = mysql_fetch_row(result);
	if ( row == NULL ) {
		mysql_free_result(result);
		return 0;
	}

	size_t id = boost::lexical_cast<size_t>(row[0]);

	mysql_free_result(result);

	return id;
}

std::vector<Gene> CDSDatabase::getGene( size_t geneID, size_t locStart, size_t locEnd ) const
{
	std::vector<Gene> genes;

	// location match totally within the location is not needed here
	//string query = "SELECT location,gene,locusTag,proteinID,product,referenceName FROM " + settings_.proteinLinkTableName_
	//		       + " WHERE geneID = " + boost::lexical_cast<string>(geneID)
    //             + " AND enclosingLocationLowerBoundary < " + boost::lexical_cast<string>(locStart)
    //             + " AND enclosingLocationUpperBoundary > " + boost::lexical_cast<string>(locEnd);

	string query = "SELECT location,gene,locusTag,proteinID,product,referenceName FROM " + settings_.proteinLinkTableName_
			     + " WHERE geneID = " + boost::lexical_cast<string>(geneID)
                 + " AND enclosingLocationUpperBoundary >= " + boost::lexical_cast<string>(locStart)
                 + " AND enclosingLocationLowerBoundary <= " + boost::lexical_cast<string>(locEnd);

	if ( mysql_query(myConnection_,query.c_str()) )
		throw GeneHunterException(mysql_error(myConnection_));

	MYSQL_RES *result = mysql_store_result(myConnection_);
	if ( result == NULL )
		throw GeneHunterException(mysql_error(myConnection_));

	while ( MYSQL_ROW row = mysql_fetch_row(result) )
	{
		if ( settings_.checkSubdividedLocation_ and !Location(row[0]).match(locStart,locEnd)) continue;

		genes.push_back(Gene(row[1],row[2],row[3],row[4],row[5]));
	}

	mysql_free_result(result);
	return genes;
}

void CDSDatabase::createIndex() const
{
	string query = "CREATE INDEX geneIDIndex ON " + settings_.proteinLinkTableName_ + " (\
    geneID)";

	if ( mysql_query(myConnection_,query.c_str()) )
	throw GeneHunterException(mysql_error(myConnection_));

	query = "CREATE INDEX enclosingLocationLowerBoundaryIndex ON " + settings_.proteinLinkTableName_ + " (\
    enclosingLocationLowerBoundary)";

	if ( mysql_query(myConnection_,query.c_str()) )
	throw GeneHunterException(mysql_error(myConnection_));

	query = "CREATE INDEX enclosingLocationUpperBoundaryIndex ON " + settings_.proteinLinkTableName_ + " (\
    enclosingLocationUpperBoundary)";

	if ( mysql_query(myConnection_,query.c_str()) )
	throw GeneHunterException(mysql_error(myConnection_));
}

string CDSDatabase::strip( string const& s ) const
{
	if (s.length() > stripBufferLength_)
		throw GeneHunterException("CDSDatabase::strip: buffer overflow.");
	mysql_real_escape_string(myConnection_, stripBuffer_, s.c_str(), s.length());
	return stripBuffer_;
}

} // namespace GeneHunter
