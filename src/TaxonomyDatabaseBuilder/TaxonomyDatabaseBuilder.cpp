#include "BrainTwister/ArgumentParser.h"
#include "GenomeLib/CDSDatabase.h"
#include "GenomeLib/CDSIterator.h"
#include "TaxonomyDatabaseBuilder/TaxonomyWriter.h"
#include "UtilitiesLib/Environment.h"
#include "UtilitiesLib/FileIO.h"
#include "UtilitiesLib/Filesystem.h"
#include "UtilitiesLib/GeneHunterException.h"
#include "UtilitiesLib/StringUtilities.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;
using namespace chrono;
using namespace GeneHunter;
namespace bt = BrainTwister;

int main( int argc, char* argv[] )
{
    try {

        const bt::ArgumentParser arg(argc, argv, version,
            {{ "gi_taxid_nucl", bt::Value<filesystem::path>(),      "Input file for taxonomy gi_taxid_nucl." },
             { "names",         bt::Value<filesystem::path>(),      "Input file for taxonomy names." },
             { "nodes",         bt::Value<filesystem::path>(),      "Input file for taxonomy nodes." }},
            {{ "database", "d", bt::Value<std::string>("Taxonomy"), "MySQL database name." }}
        );

        cout << "\n" << makeFrame("TaxonomyDatabaseBuilder version " + version, '*') << "\n" << endl;
        const auto startTime = steady_clock::now();

        filesystem::path gi_taxid_nucl_file = arg.get<filesystem::path>("gi_taxid_nucl");
        filesystem::path names_file = arg.get<filesystem::path>("names");
        filesystem::path nodes_file = arg.get<filesystem::path>("nodes");
        std::string database = arg.get<std::string>("database");

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
