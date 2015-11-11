#include "BrainTwister/ArgumentParser.h"
#include "GenomeLib/FASTQIterator.h"
#include "GenomeLib/FixedSizeHashMapSearch.h"
#include "GenomeLib/FullMatchManager.h"
#include "GenomeLib/NucleotideDatabaseIterator.h"
#include "GenomeLib/OrganismsReport.h"
#include "GenomeLib/Sequence.h"
#include "GenomeLib/TokenIterator.h"
#include "UtilitiesLib/CreateDataClass.h"
#include "UtilitiesLib/Environment.h"
#include "UtilitiesLib/FileIO.h"
#include "UtilitiesLib/GeneHunterException.h"
#include "UtilitiesLib/StringUtilities.h"
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <chrono>
#include <iostream>
#include <string>

using namespace std;
using namespace chrono;
using namespace GeneHunter;
namespace filesystem = boost::filesystem;
namespace bt = BrainTwister;

CREATE_DATA_CLASS( GeneHunterSettings,\
    (( FixedTokenSizeType, fixedTokenSize, 12 ))\
    (( FullMatchManagerSettings, fullMatchManagerSettings, FullMatchManagerSettings() ))\
    (( NucleotideDatabaseSettings, nucleotideDatabaseSettings, NucleotideDatabaseSettings() ))\
    (( OrganismsReport::Settings, organismsReportSettings, OrganismsReport::Settings() ))\
)

int main( int argc, char* argv[] )
{
    try {

        cout << "\n" << makeFrame("GeneHunter version " + version, '*') << "\n" << endl;
        const auto startTime = steady_clock::now();

        const bt::ArgumentParser arg(argc, argv, version,
            {{ "readFile",                 bt::Value<filesystem::path>(),    "Read sequence in FASTQ format." },
             { "descriptionFile",          bt::Value<filesystem::path>(),    "Description file in XML format." },
             { "outputFile",               bt::Value<filesystem::path>(),    "Output file in XML format." }},
            {{ "maxEntries", "",           bt::Value<size_t>(numeric_limits<size_t>::max()), "Define how many entries are taken from NT database." },
             { "startEntry", "",           bt::Value<size_t>(numeric_limits<size_t>::min()), "Define the index of the starting NT database entry." },
             { "maxBases", "",             bt::Value<size_t>(numeric_limits<size_t>::max()), "Define how many bases are taken from NT database." },
             { "maxBasesPerStep", "",      bt::Value<size_t>(numeric_limits<size_t>::max()), "Define how many bases are are taken from NT database for one step." },
             { "maxReads", "",             bt::Value<size_t>(numeric_limits<size_t>::max()), "Maximal number of reads in fq file." },
             { "readFullMatchManager", "", bt::Value<filesystem::path>(),    "FullMatchManager file for restart or determine taxonomy (binary or xml)." },
           //{ "threshold", "",            bt::Value<double>(),              "Threshold for match quality." },
             { "settings", "",             bt::Value<filesystem::path>(Environment::getGeneHunterRootDirectory() / "settings" / "GeneHunterSettings.xml"),
            		                                                         "Define file for specific settings (default: $GENEASSEMBLER_ROOT/settings/GeneHunterSettings.xml)." },
             { "NTDatabase", "",           bt::Value<filesystem::path>(Environment::getDatabaseFile()),
            		                                                         "Define nucleotide database file." },
             { "overwriteOutput", "",      bt::Value<bool>(),                "If set an existing output file will overwritten." },
             { "skipTaxonomy", "",         bt::Value<bool>(),                "Do not write result file, only FullMatchManager output." },
             { "onlyTaxonomy", "",         bt::Value<bool>(),                "Only write result file, do not search for new matches (is only possible together with -readFullMatchManager)." },
             { "printSteps", "",           bt::Value<bool>(),                "Printing intermediate import informations of NT database each step." },
             { "printHashMapInfo", "",     bt::Value<bool>(),                "Printing hashMap informations each step." }}
        );

        filesystem::path inputXMLFile = arg.get<filesystem::path>("descriptionFile");
        if (!exists(inputXMLFile)) throw GeneHunterException("File not found: " + inputXMLFile.string());

        filesystem::path outputXMLFile = arg.get<filesystem::path>("outputFile");
        if (!arg.get<bool>("overwriteOutput") and exists(outputXMLFile))
            throw GeneHunterException("File exist and not allowed to overwrite: " + outputXMLFile.string());

        if (arg.get<bool>("onlyTaxonomy") and !arg.get<filesystem::path>("readFullMatchManager").empty())
            throw GeneHunterException("Option 'onlyTaxonomy' can only used together with 'readFullMatchManager'.");

        // Read settings
        filesystem::path settingsFile = arg.get<filesystem::path>("settings");
        if (!exists(settingsFile)) throw GeneHunterException("Settings file " + settingsFile.string() + " not found.");
        GeneHunterSettings settings;
        readXML(settings,"GeneHunterSettings",settingsFile);

        //double threshold = arg.isOptionalFlagSet("threshold") ? boost::lexical_cast<double>(arg.getOptionalArgument("threshold")) : numeric_limits<double>::max();

        // Print settings
        cout << "Token size = " << static_cast<size_t>(settings.fixedTokenSize_) << endl;
        cout << "Compression factor of hash tokens = " << Traits<12>::HashTokenCharType::CompressionFactor << endl;
        cout << "Compression factor of reference sequence = " << Traits<12>::RefSeqCharType::CompressionFactor << endl;
        //cout << "Threshold = " << (arg.isOptionalFlagSet("threshold") ? boost::lexical_cast<string>(threshold) : "off") << endl;
        cout << "Search algorithm = " << settings.fullMatchManagerSettings_.searchAlgorithm_ << endl;
        cout << "Max number of seeds = " << settings.fullMatchManagerSettings_.maxSeeds_ << endl;
        cout << "Min number of tokens per read = " << settings.fullMatchManagerSettings_.minTokensPerRead_ << endl;
        cout << "Max bases for variants = " << settings.fullMatchManagerSettings_.maxBasesForVariants_ << endl;
        cout << "Number of nodes = " << Environment::getNbNodes() << endl;
        cout << "Number of cores per node = " << Environment::getNbCoresPerNode() << endl;
        cout << "Memory per node = " << Environment::getNodeMemoryGB() << endl;

        FullMatchManager fullMatchManager(settings.fullMatchManagerSettings_);

        if (!arg.get<filesystem::path>("readFullMatchManager").empty()) {
            readExtensionDependend(fullMatchManager, "FullMatchManager", arg.get<filesystem::path>("readFullMatchManager"));
        }

        if (!arg.get<bool>("onlyTaxonomy"))
        {
            fixedSizeHashMapSearch(settings.fixedTokenSize_, fullMatchManager, settings.nucleotideDatabaseSettings_, arg);
        }

        if (!arg.get<bool>("skipTaxonomy"))
        {
            OrganismsReport organismsReport(outputXMLFile,inputXMLFile,settings.organismsReportSettings_,
                fullMatchManager.getTotalNbImportedReadSeq());
            organismsReport.addMatches(fullMatchManager.getMatches());
        }

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

    cout << "\nGeneHunter successfully finished. Have a nice day.\n" << endl;
    return 0;

}
