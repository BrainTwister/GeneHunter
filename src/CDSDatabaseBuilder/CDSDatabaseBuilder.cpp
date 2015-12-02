#include "BrainTwister/ArgumentParser.h"
#include "GenomeLib/CDSDatabase.h"
#include "GenomeLib/CDSIterator.h"
#include "UtilitiesLib/CreateDataClass.h"
#include "UtilitiesLib/Environment.h"
#include "UtilitiesLib/FileIO.h"
#include "UtilitiesLib/Filesystem.h"
#include "UtilitiesLib/GeneHunterException.h"
#include "UtilitiesLib/StringUtilities.h"
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;
using namespace chrono;
using namespace GeneHunter;
namespace bt = BrainTwister;

CREATE_DATA_CLASS( CDSDatabaseBuilderSettings,\
    (( CDSIterator::Settings, cdsIteratorSettings, CDSIterator::Settings() ))\
)

int main( int argc, char* argv[] )
{
    try {

        cout << "\n" << makeFrame("CDSDatabaseBuilder version " + version, '*') << "\n" << endl;
        const auto startTime = steady_clock::now();

        const bt::ArgumentParser arg(argc, argv, version,
            {{ "inputFiles",     bt::Value<std::vector<filesystem::path>>(), "Input files containing CDS features (gz-files are supported)." }},
            {{ "settings", "s",  bt::Value<filesystem::path>(Environment::getGeneHunterRootDirectory() / "settings" / "CDSDatabaseBuilderSettings.xml"),
            		                                                         "File for specific settings." },
             { "tableName", "t", bt::Value<std::string>("ProteinLink"),      "Table name for MySQL database." }}
        );

        string tableName = arg.get<std::string>("tableName");
        CDSDatabase cdsDatabase(CDSDatabase::Settings(tableName,0,false));

        // Read settings
        filesystem::path settingsFile = arg.get<filesystem::path>("settings");
        if (!exists(settingsFile)) throw GeneHunterException("Settings file " + settingsFile.string() + " not found.");
        CDSDatabaseBuilderSettings settings;
        readXML(settings,"CDSDatabaseBuilderSettings",settingsFile);

        for (auto const& inputFile : arg.get<std::vector<filesystem::path>>("inputFiles"))
        {
            std::cout << "Import " << inputFile.string() << std::endl;

            std::ifstream ifs(inputFile.string());
            if (!ifs) throw GeneHunterException("Error opening file " + inputFile.string());
            boost::iostreams::filtering_streambuf<boost::iostreams::input> inbuf;
            if (inputFile.extension() == ".gz") inbuf.push(boost::iostreams::gzip_decompressor());
            inbuf.push(ifs);
            boost::shared_ptr<std::istream> ptr_instream(new std::istream(&inbuf));

            for (CDSIterator iterCur(ptr_instream, settings.cdsIteratorSettings_), iterEnd; iterCur != iterEnd; ++iterCur)
            {
                cdsDatabase.importGene(*iterCur);
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

    cout << "\nCDSDatabaseBuilder successfully finished. Have a nice day.\n" << endl;
    return 0;

}
