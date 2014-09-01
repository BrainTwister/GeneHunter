#include "Environment.h"
#include "FASTAIterator.h"
#include "FileIO.h"
#include "GeneAssemblerException.h"
#include "NucleotideDatabase.h"
#include "Power.h"
#include "StringUtilities.h"
#include <boost/filesystem.hpp>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;
using namespace GeneAssembler;
using boost::filesystem::path;

int main( int argc, char* argv[] )
{
	try {

		cout << "\n" << makeFrame("DatabaseBuilder version " + version, '*') << "\n" << endl;

		typedef Traits<12> DefaultTraits;

		if ( argc < 2 ) {
		    cout << "Usage: DatabaseBuilder <nbEntries> <nbBases> <nbBasesInFile>" << endl;
		    throw GeneAssemblerException("Wrong number of arguments.");
		}

		PtrNucleotideDatabase<DefaultTraits> ptrDatabase = PtrNucleotideDatabase<DefaultTraits>(new NucleotideDatabase<DefaultTraits>());

		NucleotideDatabaseInformation totalInfo;

		const path sharedDirectory = Environment::getSharedDirectory();
		const size_t maxFileSize = Environment::getNodeMemoryGB() * Power<1024,3>::value;

		size_t nbEntries = boost::lexical_cast<size_t>(argv[1]);
		size_t nbBases = boost::lexical_cast<size_t>(argv[2]);
		size_t nbBasesInFile = boost::lexical_cast<size_t>(argv[3]);
		size_t dbID = 0;
		size_t count = 1;

		for ( FASTAIterator<DefaultTraits::RefSeqCharType> iterCur(Environment::getDatabaseFile()), iterEnd; iterCur != iterEnd; ++iterCur, ++count )
		{
			ptrDatabase->addEntry(*iterCur);

			if ( count == nbEntries or totalInfo.getTotalNbOfBases() > nbBases) break;

			//if ( ptrDatabase->getMemoryUsage() > maxFileSize )
			if ( ptrDatabase->getInformation().getTotalNbOfBases() > nbBasesInFile )
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
}
