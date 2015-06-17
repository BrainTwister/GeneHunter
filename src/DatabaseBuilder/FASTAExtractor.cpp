#include "ArgumentInterpreter.h"
#include "Environment.h"
#include "FASTAIterator.h"
#include "GeneHunterException.h"
#include "StringUtilities.h"
#include <boost/filesystem.hpp>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;
using namespace chrono;
using namespace GeneHunter;
using boost::filesystem::path;

int main( int argc, char* argv[] )
{
    try {

        cout << "\n" << makeFrame("FASTAExtractor version 1.0 revision 412", '*') << "\n" << endl;
        const auto startTime = steady_clock::now();

        ArgumentInterpreter arg(argc,argv,
            {{ "geneID", ArgumentInterpreter::NonOptional, "Gene ID number." },
             { "start",  ArgumentInterpreter::NonOptional, "Position of first base." },
             { "length", ArgumentInterpreter::NonOptional, "Length of sequence to extract." }}
        );

        size_t geneID = boost::lexical_cast<size_t>(arg.getNonOptionalArgument("geneID"));
        size_t start = boost::lexical_cast<size_t>(arg.getNonOptionalArgument("start"));
        size_t length = boost::lexical_cast<size_t>(arg.getNonOptionalArgument("length"));

        string line;
        string sequence;
        ifstream is(Environment::getDatabaseFile().c_str());

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
