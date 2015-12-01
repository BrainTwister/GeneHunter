#include "FASTAIterator.h"
#include "FileIO.h"
#include "GenomeTest.h"
#include "NucleotideDatabase.h"
#include <boost/filesystem/path.hpp>

using namespace std;
using namespace GeneAssembler;
using boost::filesystem::path;

void ReferenceDatabaseTest::operator () ( TestSuite& testSuite ) const
{
	typedef Traits<24,CompressedCharFourInOne,CompressedCharTwoInOne> TraitsType;
	typedef TraitsType::HashTokenType HashTokenType;
	typedef TraitsType::RefSeqCharType RefSeqCharType;

	{
		NucleotideDatabase<TraitsType> db;

		for ( FASTAIterator<RefSeqCharType> iterCur(testSuite.getInputDirectory() / "nt_head"), iterEnd;
			iterCur != iterEnd; ++iterCur )
		{
			db.addEntry(*iterCur);
		}

		//db.printHashMapReferenceSequenceTokens();

		if ( ! db.find(HashTokenType("GCTGCACCAACAATAAATCAGTTT"))) testSuite.errorLog( name() + ": token not found");
		if (   db.find(HashTokenType("GCTGCACCAACAATAAATCAGTTA"))) testSuite.errorLog( name() + ": wrong token found");

		path dbOutputFile = testSuite.getOutputDirectory() / "NucleotideDatabase1.bin";
		writeBinary(db,"NucleotideDatabase",dbOutputFile);
	}
	{
		NucleotideDatabase<TraitsType> db;

		path dbOutputFile = testSuite.getOutputDirectory() / "NucleotideDatabase1.bin";
		readBinary(db,"NucleotideDatabase",dbOutputFile);

		if ( ! db.find(HashTokenType("GCTGCACCAACAATAAATCAGTTT"))) testSuite.errorLog( name() + ": token not found 2");
		if (   db.find(HashTokenType("GCTGCACCAACAATAAATCAGTTA"))) testSuite.errorLog( name() + ": wrong token found 2");
	}
}
