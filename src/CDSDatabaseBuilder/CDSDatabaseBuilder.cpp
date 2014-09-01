#include "ArgumentInterpreter.h"
#include "CreateDataClass.h"
#include "Environment.h"
#include "FileIO.h"
#include "CDSDatabase.h"
#include "CDSIterator.h"
#include "GeneAssemblerException.h"
#include "StringUtilities.h"
#include <boost/filesystem.hpp>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;
using namespace chrono;
using namespace GeneAssembler;
using boost::filesystem::path;

CREATE_DATA_CLASS( CDSDatabaseBuilderSettings,\
	(( CDSIterator::Settings, cdsIteratorSettings, CDSIterator::Settings() ))\
)

int main( int argc, char* argv[] )
{
	try {

		cout << "\n" << makeFrame("CDSDatabaseBuilder version " + version, '*') << "\n" << endl;
		const auto startTime = steady_clock::now();

		const ArgumentInterpreter arg(argc,argv,
			{{ "inputFiles", ArgumentInterpreter::NonOptionalList, "Input files containing CDS features (gz-files are supported)." },
			 { "settings",   ArgumentInterpreter::Optional,        "File for specific settings (default: $GENEASSEMBLER_ROOT/settings/CDSDatabaseBuilderSettings.xml)." },
			 { "tableName",  ArgumentInterpreter::Optional,        "Table name for MySQL database (default: ProteinLink)." }}
		);

		string tableName = arg.isOptionalFlagSet("tableName") ? arg.getOptionalArgument("tableName") : "ProteinLink";
		bool onlyGenes = arg.isOptionalFlagSet("onlyGenes") ? true : false;

		CDSDatabase cdsDatabase(CDSDatabase::Settings(tableName,0,false));

		// Read settings
		path settingsFile = Environment::getGeneAssemblerRootDirectory() / "settings" / "CDSDatabaseBuilderSettings.xml";
		if (arg.isOptionalFlagSet("settings")) settingsFile = path(arg.getOptionalArgument("settings"));
        if (!exists(settingsFile)) throw GeneAssemblerException("Settings file " + settingsFile.string() + " not found.");
        CDSDatabaseBuilderSettings settings;
	    readXML(settings,"CDSDatabaseBuilderSettings",settingsFile);

		for ( auto const& inputFile : arg.getNonOptionalList() )
		{
			string filename(inputFile);
			cout << "Import " << filename << endl;

			bool fileIsZipped = false;
			if ( filename.substr(filename.size()-3,3) == ".gz" ) {
				string cmd = "gunzip " + filename;
				cout << cmd << endl;
				system(cmd.c_str());
				filename.erase(filename.size()-3,3);
				fileIsZipped = true;
			}

			for ( CDSIterator iterCur(filename,settings.cdsIteratorSettings_), iterEnd; iterCur != iterEnd; ++iterCur )
			{
				cdsDatabase.importGene(*iterCur);
			}

			if ( fileIsZipped ) {
				string cmd = string("gzip ") + filename;
				cout << cmd << endl;
				system(cmd.c_str());
			}
		}

		cout << "Number of entries in CDSDatabase = " << cdsDatabase.nbGeneEntries() << endl;
		cout << "Number of proteinLinks in CDSDatabase = " << cdsDatabase.nbProteinLinks() << endl;

		cdsDatabase.createIndex();

		const auto stopTime = steady_clock::now();
		const auto duration = stopTime - startTime;
		cout << "Total time (hh:mm:ss): "
			 << setfill('0') << setw(2) << duration_cast<hours>(duration).count() << ":"
			 << setfill('0') << setw(2) << duration_cast<minutes>(duration % hours(1)).count() << ":"
			 << setfill('0') << setw(2) << duration_cast<seconds>(duration % minutes(1)).count() << endl;

	} catch ( GeneAssemblerException const& e ) {
		cout << "GeneAssembler exception: " << e.what() << endl;
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

	cout << "\nCDSDatabaseBuilder successfully finished. Have a nice day.\n" << endl;
	return 0;

}
