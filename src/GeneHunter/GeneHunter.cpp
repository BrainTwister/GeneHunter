#include "ArgumentInterpreter.h"
#include "CreateDataClass.h"
#include "Environment.h"
#include "FASTQIterator.h"
#include "FileIO.h"
#include "FixedSizeHashMapSearch.h"
#include "FullMatchManager.h"
#include "GeneHunterException.h"
#include "NucleotideDatabaseIterator.h"
#include "OrganismsReport.h"
#include "Sequence.h"
#include "StringUtilities.h"
#include "TokenIterator.h"
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <chrono>
#include <iostream>
#include <string>

using namespace std;
using namespace chrono;
using namespace GeneHunter;
using boost::filesystem::path;

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

        ArgumentInterpreter arg(argc,argv,
            {{ "readFile",             ArgumentInterpreter::NonOptional, "Read sequence in FASTQ format." },
             { "descriptionFile",      ArgumentInterpreter::NonOptional, "Description file in XML format." },
             { "outputFile",           ArgumentInterpreter::NonOptional, "Output file in XML format." },
             { "maxEntries",           ArgumentInterpreter::Optional,    "Define how many entries are taken from NT database." },
             { "startEntry",           ArgumentInterpreter::Optional,    "Define the index of the starting NT database entry." },
             { "maxBases",             ArgumentInterpreter::Optional,    "Define how many bases are taken from NT database." },
             { "maxBasesPerStep",      ArgumentInterpreter::Optional,    "Define how many bases are are taken from NT database for one step." },
             { "maxReads",             ArgumentInterpreter::Optional,    "Maximal number of reads in fq file." },
             { "readFullMatchManager", ArgumentInterpreter::Optional,    "FullMatchManager file for restart or determine taxonomy (binary or xml)." },
             //{ "threshold",            ArgumentInterpreter::Optional,    "Threshold for match quality." },
             { "settings",             ArgumentInterpreter::Optional,    "Define file for specific settings (default: $GENEASSEMBLER_ROOT/settings/GeneHunterSettings.xml)." },
             { "NTDatabase",           ArgumentInterpreter::Optional,    "Define nucleotide database file." },
             { "overwriteOutput",      ArgumentInterpreter::Boolean,     "If set an existing output file will overwritten." },
             { "skipTaxonomy",         ArgumentInterpreter::Boolean,     "Do not write result file, only FullMatchManager output." },
             { "onlyTaxonomy",         ArgumentInterpreter::Boolean,     "Only write result file, do not search for new matches (is only possible together with -readFullMatchManager)." },
             { "printSteps",           ArgumentInterpreter::Boolean,     "Printing intermediate import informations of NT database each step." },
             { "printHashMapInfo",     ArgumentInterpreter::Boolean,     "Printing hashMap informations each step." }}
        );

        path inputXMLFile = arg.getNonOptionalArgument("descriptionFile");
        if (!exists(inputXMLFile)) throw GeneHunterException("File not found: " + inputXMLFile.string());

        path outputXMLFile = arg.getNonOptionalArgument("outputFile");
        if (!arg.isBooleanFlagSet("overwriteOutput") and exists(outputXMLFile))
            throw GeneHunterException("File exist and not allowed to overwrite: " + outputXMLFile.string());

        if ( arg.isBooleanFlagSet("onlyTaxonomy") and !arg.isOptionalFlagSet("readFullMatchManager") )
            throw GeneHunterException("Option 'onlyTaxonomy' can only used together with 'readFullMatchManager'.");

        // Read settings
        path settingsFile = Environment::getGeneHunterRootDirectory() / "settings" / "GeneHunterSettings.xml";
        if (arg.isOptionalFlagSet("settings")) settingsFile = path(arg.getOptionalArgument("settings"));
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

        if (arg.isOptionalFlagSet("readFullMatchManager")) {
            readExtensionDependend(fullMatchManager,"FullMatchManager",arg.getOptionalArgument("readFullMatchManager"));
        }

        if (!arg.isBooleanFlagSet("onlyTaxonomy")) {
            fixedSizeHashMapSearch(settings.fixedTokenSize_,fullMatchManager,settings.nucleotideDatabaseSettings_,arg);
        }

        if (!arg.isBooleanFlagSet("skipTaxonomy"))
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
