#include "CharTypes.h"
#include "Sequence.h"
#include "gtest/gtest.h"

using namespace std;
using namespace GeneHunter;

TEST(SequenceTest,TestChar1)
{
	string seqString("AGCT");
	Sequence<char> seq(seqString);
	EXPECT_EQ("AGCT",seq.getString());
}

TEST(SequenceTest,TestChar2)
{
	string seqString("agct");
	Sequence<char> seq(seqString);
	EXPECT_EQ("AGCT",seq.getString());
}

#if 0
	{
		string seqString("AGCT");
		Sequence<CompressedCharTwoInOne> seq(seqString);
		if ( seq.getString() != "AGCT" ) testSuite.errorLog(name() + ": error CompressedCharTwoInOne " + seqString);
	}
	{
		string seqString("AGCTA");
		Sequence<CompressedCharTwoInOne> seq(seqString);
		if ( seq.getString() != "AGCTAX" ) testSuite.errorLog(name() + ": error CompressedCharTwoInOne " + seqString);
	}
	{
		string seqString("AGCT");
		Sequence<CompressedCharFourInOne> seq(seqString);
		if ( seq.getString() != "AGCT" ) testSuite.errorLog(name() + ": error CompressedCharFourInOne " + seqString);
	}
	{
		string seqString("agct");
		Sequence<CompressedCharFourInOne> seq(seqString);
		if ( seq.getString() != "AGCT" ) testSuite.errorLog(name() + ": error CompressedCharFourInOne " + seqString);
	}
	{
		string seqString("AGCTG");
		Sequence<CompressedCharFourInOne> seq(seqString);
		if ( seq.getString() != "AGCTGAAA" ) testSuite.errorLog(name() + ": error CompressedCharFourInOne " + seqString);
	}
	{
		CompressedCharTwoInOne cc{'A','G'};
		if ( ! cc.containOnlyPureBases() ) testSuite.errorLog(name() + ": error CompressedCharTwoInOne containOnlyPureBases AG");
	}
	{
		CompressedCharTwoInOne cc{'N','G'};
		if ( cc.containOnlyPureBases() ) testSuite.errorLog(name() + ": error CompressedCharTwoInOne containOnlyPureBases NG");
	}
	{
		CompressedCharTwoInOne cc{'T','N'};
		if ( cc.containOnlyPureBases() ) testSuite.errorLog(name() + ": error CompressedCharTwoInOne containOnlyPureBases TN");
	}
	{
		Sequence<CompressedCharTwoInOne> s1("ACGTKSRMWYBDHVNXacgtksrmwybdhvnx");
		Sequence<char> s2(s1);

		if ( static_cast<int>('a') !=  97 ) testSuite.errorLog(name() + ": error static_cast<int>('a') == 97");
		if ( static_cast<int>('c') !=  99 ) testSuite.errorLog(name() + ": error static_cast<int>('c') == 99");
		if ( static_cast<int>('g') != 103 ) testSuite.errorLog(name() + ": error static_cast<int>('g') == 103");
		if ( static_cast<int>('t') != 116 ) testSuite.errorLog(name() + ": error static_cast<int>('t') == 116");
		if ( static_cast<int>('k') != 107 ) testSuite.errorLog(name() + ": error static_cast<int>('k') == 107");
		if ( static_cast<int>('s') != 115 ) testSuite.errorLog(name() + ": error static_cast<int>('s') == 115");
		if ( static_cast<int>('r') != 114 ) testSuite.errorLog(name() + ": error static_cast<int>('r') == 114");
		if ( static_cast<int>('m') != 109 ) testSuite.errorLog(name() + ": error static_cast<int>('m') == 109");
		if ( static_cast<int>('w') != 119 ) testSuite.errorLog(name() + ": error static_cast<int>('w') == 119");
		if ( static_cast<int>('y') != 121 ) testSuite.errorLog(name() + ": error static_cast<int>('y') == 121");
		if ( static_cast<int>('b') !=  98 ) testSuite.errorLog(name() + ": error static_cast<int>('b') == 98");
		if ( static_cast<int>('d') != 100 ) testSuite.errorLog(name() + ": error static_cast<int>('d') == 100");
		if ( static_cast<int>('h') != 104 ) testSuite.errorLog(name() + ": error static_cast<int>('h') == 104");
		if ( static_cast<int>('v') != 118 ) testSuite.errorLog(name() + ": error static_cast<int>('v') == 118");
		if ( static_cast<int>('n') != 110 ) testSuite.errorLog(name() + ": error static_cast<int>('n') == 110");
		if ( static_cast<int>('x') != 120 ) testSuite.errorLog(name() + ": error static_cast<int>('x') == 120");

		if ( s2.getString() != "ACGTKSRMWYBDHVNXACGTKSRMWYBDHVNX" )
			testSuite.errorLog(name() + ": error convert CompressedCharTwoInOne into char");
	}
	{
		Sequence<CompressedCharFourInOne> s1("AGCTagct");
		Sequence<char> s2(s1);

		if ( s2.getString() != "AGCTAGCT" )
			testSuite.errorLog(name() + ": error convert CompressedCharFourInOne into char");
	}
//	{
//		Sequence<CompressedCharTwoInOne> s1("AGCTagct");
//		Sequence<CompressedCharFourInOne> s2(s1);
//
//		if ( s1.getString() != s2.getString() )
//			testSuite.errorLog(name() + ": error convert CompressedCharTwoInOne into CompressedCharFourInOne");
//	}
	{
		Sequence<char> s1("AGTCctga");
		Sequence<char> s2 = reverse(s1);
		Sequence<char> s3 = complement(s1);
		Sequence<char> s4 = reverseComplement(s1);

		if ( s2.getString() != "AGTCCTGA" ) testSuite.errorLog(name() + ": error reverse char");
		if ( s3.getString() != "TCAGGACT" ) testSuite.errorLog(name() + ": error complement char");
	}
}
#endif
