#include "BrainTwister/ArgumentParser.h"
#include "GenomeLib/FASTAIterator.h"
#include "GenomeLib/NucleotideDatabase.h"
#include "UtilitiesLib/FileIO.h"
#include <UtilitiesLib/Filesystem.h>
#include "UtilitiesLib/GeneHunterException.h"
#include "UtilitiesLib/StringUtilities.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;
using namespace GeneHunter;
namespace bt = BrainTwister;

int main(int argc, char* argv[])
{
    try {

        cout << "\n" << makeFrame("DatabaseBuilder version " + version, '*') << "\n" << endl;

        const bt::ArgumentParser arg(argc, argv, version, {},
            {{ "input", "i",    bt::Value<filesystem::path>("nt.gz"), "Input file." },
             { "output", "o",   bt::Value<filesystem::path>("."),     "Output directory." },
			 { "nbEntries",     bt::Value<size_t>(numeric_limits<size_t>::max()), "Number of entries to collect." },
             { "nbBases",       bt::Value<size_t>(numeric_limits<size_t>::max()), "Number of nucleotide bases to collect." },
             { "nbBasesInFile", bt::Value<size_t>(1e9),                           "Number of nucleotide bases per file." }}
        );

        typedef Traits<12> DefaultTraits;

        PtrNucleotideDatabase<DefaultTraits> ptrDatabase = PtrNucleotideDatabase<DefaultTraits>(new NucleotideDatabase<DefaultTraits>());
        NucleotideDatabaseInformation totalInfo;

        size_t dbID = 0;
        size_t count = 1;
        for (FASTAIterator<DefaultTraits::RefSeqCharType> iterCur(arg.get<filesystem::path>("input")),
        	iterEnd; iterCur != iterEnd; ++iterCur, ++count)
        {
            ptrDatabase->addEntry(*iterCur);

            if (count == arg.get<size_t>("nbEntries") or totalInfo.getTotalNbOfBases() > arg.get<size_t>("nbBases")) break;

            if (ptrDatabase->getInformation().getTotalNbOfBases() > arg.get<size_t>("nbBasesInFile"))
            {
                std::string filename = "NucleotideDatabase" + std::to_string(dbID);

                filesystem::path dbOutputFile = arg.get<filesystem::path>("output") / (filename + ".bin");
                writeBinary(*ptrDatabase, "NucleotideDatabase", dbOutputFile);

                filesystem::path hashMapInfoFile = arg.get<filesystem::path>("output") / (filename + ".info");
                ofstream ofs(hashMapInfoFile.c_str());
                ptrDatabase->printHashMapReferenceSequenceState(ofs);

                totalInfo.merge(ptrDatabase->getInformation());
                ++dbID;
                ptrDatabase = PtrNucleotideDatabase<DefaultTraits>(new NucleotideDatabase<DefaultTraits>());
            }
        }

        std::string filename = "NucleotideDatabase" + std::to_string(dbID);

        filesystem::path dbOutputFile = arg.get<filesystem::path>("output") / (filename + ".bin");
        writeBinary(*ptrDatabase, "NucleotideDatabase", dbOutputFile);

        filesystem::path hashMapInfoFile = arg.get<filesystem::path>("output") / (filename + ".info");
        ofstream ofs(hashMapInfoFile.c_str());
        ptrDatabase->printHashMapReferenceSequenceState(ofs);

        totalInfo.merge(ptrDatabase->getInformation());
        std::cout << totalInfo << std::endl;

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
}
