#include "BrainTwister/ArgumentParser.h"
#include "GenomeLib/FASTAIterator.h"
#include "GenomeLib/NucleotideDatabase.h"
#include "UtilitiesLib/Environment.h"
#include "UtilitiesLib/FileIO.h"
#include "UtilitiesLib/GeneHunterException.h"
#include "UtilitiesLib/StringUtilities.h"
#include <boost/filesystem.hpp>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;
using namespace GeneHunter;
namespace filesystem = boost::filesystem;
namespace bt = BrainTwister;

int main(int argc, char* argv[])
{
    try {

        const bt::ArgumentParser arg(argc, argv, version,
            {{ "nbEntries",     bt::Value<size_t>(),                  "Number of entries to collect." },
             { "nbBases",       bt::Value<size_t>(),                  "Number of nucleotide bases to collect." },
             { "nbBasesInFile", bt::Value<size_t>(),                  "Number of nucleotide bases per file." }},
            {{ "input", "i",    bt::Value<filesystem::path>("nt.gz"), "Input file." },
             { "output", "o",   bt::Value<filesystem::path>("."),     "Output directory." }}
        );

        cout << "\n" << makeFrame("DatabaseBuilder version " + version, '*') << "\n" << endl;

        typedef Traits<12> DefaultTraits;

        if ( argc < 2 ) {
            cout << "Usage: DatabaseBuilder <nbEntries> <nbBases> <nbBasesInFile>" << endl;
            throw GeneHunterException("Wrong number of arguments.");
        }

        PtrNucleotideDatabase<DefaultTraits> ptrDatabase = PtrNucleotideDatabase<DefaultTraits>(new NucleotideDatabase<DefaultTraits>());
        NucleotideDatabaseInformation totalInfo;

        size_t dbID = 0;
        size_t count = 1;
        for (FASTAIterator<DefaultTraits::RefSeqCharType> iterCur(Environment::getDatabaseFile()),
        	iterEnd; iterCur != iterEnd; ++iterCur, ++count)
        {
            ptrDatabase->addEntry(*iterCur);

            if (count == arg.get<int>("nbEntries") or totalInfo.getTotalNbOfBases() > arg.get<int>("nbBases")) break;

            if (ptrDatabase->getInformation().getTotalNbOfBases() > arg.get<int>("nbBasesInFile"))
            {
                filesystem::path dbOutputFile = Environment::getSharedDirectory() / string("NucleotideDatabase" + boost::lexical_cast<string>(dbID) + ".bin");
                writeBinary(*ptrDatabase,"NucleotideDatabase",dbOutputFile);

                filesystem::path hashMapInfoFile = Environment::getSharedDirectory() / string("NucleotideDatabase" + boost::lexical_cast<string>(dbID) + ".info");
                ofstream ofs(hashMapInfoFile.c_str());
                ptrDatabase->printHashMapReferenceSequenceState(ofs);

                totalInfo.merge(ptrDatabase->getInformation());
                ++dbID;
                ptrDatabase = PtrNucleotideDatabase<DefaultTraits>(new NucleotideDatabase<DefaultTraits>());
            }
        }

        filesystem::path dbOutputFile = Environment::getSharedDirectory() / string("NucleotideDatabase" + boost::lexical_cast<string>(dbID) + ".bin");
        writeBinary(*ptrDatabase,"NucleotideDatabase",dbOutputFile);

        filesystem::path hashMapInfoFile = Environment::getSharedDirectory() / string("NucleotideDatabase" + boost::lexical_cast<string>(dbID) + ".info");
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
