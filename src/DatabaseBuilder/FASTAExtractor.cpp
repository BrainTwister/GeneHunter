#include "BrainTwister/ArgumentParser.h"
#include "GenomeLib/FASTAIterator.h"
#include "UtilitiesLib/Filesystem.h"
#include "UtilitiesLib/GeneHunterException.h"
#include "UtilitiesLib/StringUtilities.h"
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;
using namespace chrono;
using namespace GeneHunter;
namespace bt = BrainTwister;

int main( int argc, char* argv[] )
{
    try {

        cout << "\n" << makeFrame("DatabaseBuilder version " + version, '*') << "\n" << endl;
        const auto startTime = steady_clock::now();

        const bt::ArgumentParser arg(argc, argv, version,
            {{ "input",  bt::Value<filesystem::path>(), "Input file." },
             { "geneID", bt::Value<size_t>(),           "Gene ID number." },
             { "start",  bt::Value<size_t>(),           "Position of first base." },
             { "length", bt::Value<size_t>(),           "Length of sequence to extract." }}
        );

        size_t geneID = arg.get<size_t>("geneID");
        size_t start = arg.get<size_t>("start");
        size_t length = arg.get<size_t>("length");

        string line;
        string sequence;
        ifstream is(arg.get<filesystem::path>("input").c_str());

        while ( std::getline(is,line) )
        {
            boost::algorithm::trim(line);
            if ( line[0] == '>' and FASTA<char>::getFirstGeneID(line) == geneID )
            {
                while ( std::getline(is,line) )
                {
                    if ( line[0] == '>' ) break;
                    sequence += line;
                }
                break;
            }
        }

        cout << sequence.substr(start,length) << endl;

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

    cout << "\nFASTAExtractor successfully finished. Have a nice day.\n" << endl;
    return 0;

}
