#include "CharTypes.h"
#include "FileIO.h"
#include "GenomeTest.h"
#include "SequenceToken.h"
#include <unordered_map>

using namespace std;
using namespace GeneAssembler;

void SequenceTokenTest::operator () ( TestSuite& testSuite ) const
{
	{
		string seqString{"ACGT"};
		SequenceToken<char,4> token(seqString.begin(),seqString.end());
		if ( token.getString() != "ACGT" ) testSuite.errorLog("GeneSequenceTest: error 1");
	}
	{
		string seqString{"CACGGTACCAAAACACCTTGATTA"};
		SequenceToken<CompressedCharTwoInOne,24> token(seqString.begin(),seqString.end());
		if ( token.getString() != "CACGGTACCAAAACACCTTGATTA" ) testSuite.errorLog("GeneSequenceTest: error CompressedCharTwoInOne 1");

		boost::filesystem::path dbOutputFile = testSuite.getOutputDirectory() / "SequenceToken2.bin";
		writeBinary(token,"SequenceToken",dbOutputFile);
	}
	{
		SequenceToken<CompressedCharTwoInOne,24> token;

		boost::filesystem::path dbOutputFile = testSuite.getOutputDirectory() / "SequenceToken2.bin";
		readBinary(token,"SequenceToken",dbOutputFile);

		if ( token.getString() != "CACGGTACCAAAACACCTTGATTA" ) testSuite.errorLog("GeneSequenceTest: error CompressedCharTwoInOne 2");
	}
	{
		string seqString{"CACGGTACCAAAACACCTTGATTA"};
		SequenceToken<CompressedCharFourInOne,24> token(seqString.begin(),seqString.end());
		if ( token.getString() != "CACGGTACCAAAACACCTTGATTA" ) testSuite.errorLog("GeneSequenceTest: error CompressedCharTwoInOne 1");

		boost::filesystem::path dbOutputFile = testSuite.getOutputDirectory() / "SequenceToken3.bin";
		writeBinary(token,"SequenceToken",dbOutputFile);
	}
	{
		SequenceToken<CompressedCharFourInOne,24> token;

		boost::filesystem::path dbOutputFile = testSuite.getOutputDirectory() / "SequenceToken3.bin";
		readBinary(token,"SequenceToken",dbOutputFile);

		if ( token.getString() != "CACGGTACCAAAACACCTTGATTA" ) testSuite.errorLog("GeneSequenceTest: error CompressedCharTwoInOne 2");
	}
	{
		string seqString{"ACGTA"};

		// Next line must not compile!
		//SequenceToken<CompressedCharTwoInOne,2> token(seqString.begin(),seqString.end());
	}
	{
		string test("ABABABABABABABABABABABAB");
		SequenceToken<char,12> token1( test.begin(), test.begin()+12 );
		if ( token1.getString() != "ABABABABABAB" ) testSuite.errorLog("GeneSequenceTest: error 2/1");

		SequenceToken<char,12> token2(token1);
		if ( token2.getString() != "ABABABABABAB" ) testSuite.errorLog("GeneSequenceTest: error 2/2");

		SequenceToken<char,12> token3 = token2;
		if ( token3.getString() != "ABABABABABAB" ) testSuite.errorLog("GeneSequenceTest: error 2/3");
	}
	{
		typedef SequenceToken<char,12> SequenceTokenType;
		typedef std::unordered_multimap<SequenceTokenType,int,std::hash<SequenceTokenType>,std::equal_to<SequenceTokenType> > HashMap;

		HashMap hashMap;
		hashMap.reserve(100000);

		//cout << "bucketCount = " << hashMap.bucket_count() << endl;

		SequenceToken<char,12> token1("GACTGACTGACT");
		hashMap.insert(std::make_pair(token1,1));

		//cout << "bucketCount = " << hashMap.bucket_count() << endl;
	}
	{
		Sequence<char> s1("AGCT");
		SequenceToken<CompressedCharFourInOne,4> s2(s1.begin(),s1.end());

		if ( s1.getString() != s2.getString() )
			testSuite.errorLog(name() + ": error convert char into CompressedCharFourInOne");
	}
	{
		Sequence<CompressedCharTwoInOne> s1("AGCT");
		SequenceToken<CompressedCharFourInOne,4> s2(s1.begin(),s1.end());

		if ( s1.getString() != s2.getString() )
			testSuite.errorLog(name() + ": error convert CompressedCharTwoInOne into CompressedCharFourInOne");
	}
	{
		SequenceToken<CompressedCharFourInOne,4> s1("AGCT");
		SequenceToken<CompressedCharFourInOne,4> s2 = complement(s1);

		if ( s2.getString() != "TCGA" )
			testSuite.errorLog(name() + ": error convert CompressedCharTwoInOne into CompressedCharFourInOne");
	}
}
