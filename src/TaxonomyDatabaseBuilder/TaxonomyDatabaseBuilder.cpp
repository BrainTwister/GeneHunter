#include "ArgumentInterpreter.h"
#include "Environment.h"
#include "FileIO.h"
#include "CDSDatabase.h"
#include "CDSIterator.h"
#include "GeneHunterException.h"
#include "StringUtilities.h"
#include "TaxonomyWriter.h"
#include <boost/filesystem.hpp>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;
using namespace chrono;
using namespace GeneHunter;
using boost::filesystem::path;

int main( int argc, char* argv[] )
{
	try {

		cout << "\n" << makeFrame("TaxonomyDatabaseBuilder version " + version, '*') << "\n" << endl;
		const auto startTime = steady_clock::now();

		const ArgumentInterpreter arg(argc,argv,
			{{ "gi_taxid_nucl", ArgumentInterpreter::NonOptional, "Input file for taxonomy gi_taxid_nucl." },
			 { "names",         ArgumentInterpreter::NonOptional, "Input file for taxonomy names." },
			 { "nodes",         ArgumentInterpreter::NonOptional, "Input file for taxonomy nodes." },
			 { "database",      ArgumentInterpreter::Optional,    "Define MySQL database." }}
		);

		path gi_taxid_nucl_file = arg.getNonOptionalArgument("gi_taxid_nucl");
		path names_file = arg.getNonOptionalArgument("names");
		path nodes_file = arg.getNonOptionalArgument("nodes");
		string database = arg.isOptionalFlagSet("database") ? arg.getOptionalArgument("database") : "Taxonomy";

		TaxonomyWriter(gi_taxid_nucl_file,names_file,nodes_file,database);

		const auto stopTime = steady_clock::now();
		const auto duration = stopTime - startTime;
		cout << "Total time (hh:mm:ss): "
			 << setfill('0') << setw(2) << duration_cast<hours>(duration).count() << ":"
			 << setfill('0') << setw(2) << duration_cast<minutes>(duration % hours(1)).count() << ":"
			 << setfill('0') << setw(2) << duration_cast<seconds>(duration % minutes(1)).count() << endl;

	} catch ( GeneHunterException const& e ) {
		cout << "GeneHunter exception: " << e.what() << endl;
		cout << "Program was aborted." << endl;
		return 1;
	} catch ( std::exception const& e ) {
		cout << "Standard exception: " << e.what() << endl;
		cout << "Program was aborted." << endl;
		return 1;
	} catch ( ... ) {
		cout << "Unknown exception." << endl;
		cout << "Program was aborted." << endl;
		return 1;
	}

	cout << "\nTaxonomyDatabaseBuilder successfully finished. Have a nice day.\n" << endl;
	return 0;

}
