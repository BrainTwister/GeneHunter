#include "BrainTwister/ArgumentParser.h"
#include "Environment.h"
#include "FASTAIterator.h"
#include "FileIO.h"
#include "GeneHunterException.h"
#include "NucleotideDatabase.h"
#include "Power.h"
#include "StringUtilities.h"
#include <boost/filesystem.hpp>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;
using namespace GeneHunter;
using boost::filesystem::path;
namespace bt = BrainTwister;

int main(int argc, char* argv[])
{
    try {

        bt::ArgumentParser arg(argc, argv, version,
            {{"input", "i", bt::Value<std::string>("nt.gz"), "Input file."},
             {"output", "o", bt::Value<std::string>("."), "Output directory."}},
            {{"nbEntries", bt::Value<size_t>(), "Number of entries to collect."},
             {"nbBases", bt::Value<size_t>(), "Number of nucleotide bases to collect."},
             {"nbBasesInFile", bt::Value<size_t>(), "Number of nucleotide bases per file."}}
        );

        cout << "\n" << makeFrame("DatabaseBuilder version " + version, '*') << "\n" << endl;
        const auto startTime = steady_clock::now();

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
                path dbOutputFile = sharedDirectory / string("NucleotideDatabase" + boost::lexical_cast<string>(dbID) + ".bin");
                writeBinary(*ptrDatabase,"NucleotideDatabase",dbOutputFile);

                path hashMapInfoFile = sharedDirectory / string("NucleotideDatabase" + boost::lexical_cast<string>(dbID) + ".info");
                ofstream ofs(hashMapInfoFile.c_str());
                ptrDatabase->printHashMapReferenceSequenceState(ofs);

                totalInfo.merge(ptrDatabase->getInformation());
                ++dbID;
                ptrDatabase = PtrNucleotideDatabase<DefaultTraits>(new NucleotideDatabase<DefaultTraits>());
            }
        }

        path dbOutputFile = sharedDirectory / string("NucleotideDatabase" + boost::lexical_cast<string>(dbID) + ".bin");
        writeBinary(*ptrDatabase,"NucleotideDatabase",dbOutputFile);

        path hashMapInfoFile = sharedDirectory / string("NucleotideDatabase" + boost::lexical_cast<string>(dbID) + ".info");
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
