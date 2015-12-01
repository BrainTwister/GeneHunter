#include "GenomeLib/CharTypes.h"
#include "GenomeLib/Sequence.h"
#include "gtest/gtest.h"

using namespace std;
using namespace GeneHunter;

TEST(SequenceTest, char)
{
	using T = char;

    EXPECT_EQ("AGCT", Sequence<T>("AGCT").getString());
    EXPECT_EQ("AGCT", Sequence<T>("agct").getString());

    Sequence<T> s1("AGTCctga");
    Sequence<T> s2 = reverse(s1);
    Sequence<T> s3 = complement(s1);
    Sequence<T> s4 = reverseComplement(s1);

    EXPECT_EQ("AGTCCTGA", s2.getString());
    EXPECT_EQ("TCAGGACT", s3.getString());
    EXPECT_EQ("TCAGGACT", s4.getString());
}

TEST(SequenceTest, CompressedCharTwoInOne)
{
	using T = CompressedCharTwoInOne;

    EXPECT_EQ("AGCT", Sequence<T>("AGCT").getString());
    EXPECT_EQ("AGCT", Sequence<T>("agct").getString());
    EXPECT_EQ("AGCTAX", Sequence<T>("agcta").getString());
    EXPECT_EQ("AGCTAX", Sequence<T>("agcta").getString());

    T c1{'A','G'};
    EXPECT_TRUE(c1.containOnlyPureBases());
    T c2{'N','G'};
    EXPECT_FALSE(c2.containOnlyPureBases());
    T c3{'T','N'};
    EXPECT_FALSE(c3.containOnlyPureBases());
}

TEST(SequenceTest, CompressedCharFourInOne)
{
	using T = CompressedCharFourInOne;

    EXPECT_EQ("AGCT", Sequence<T>("AGCT").getString());
    EXPECT_EQ("AGCT", Sequence<T>("agct").getString());
    EXPECT_EQ("AGCTGAAA", Sequence<T>("agctg").getString());
    EXPECT_EQ("AGCTAAAA", Sequence<T>("agcta").getString());
}

TEST(SequenceTest, charCode)
{
	EXPECT_EQ(static_cast<int>('a'),  97);
	EXPECT_EQ(static_cast<int>('c'),  99);
	EXPECT_EQ(static_cast<int>('g'), 103);
	EXPECT_EQ(static_cast<int>('t'), 116);
	EXPECT_EQ(static_cast<int>('k'), 107);
	EXPECT_EQ(static_cast<int>('s'), 115);
	EXPECT_EQ(static_cast<int>('r'), 114);
	EXPECT_EQ(static_cast<int>('m'), 109);
	EXPECT_EQ(static_cast<int>('w'), 119);
	EXPECT_EQ(static_cast<int>('y'), 121);
	EXPECT_EQ(static_cast<int>('b'),  98);
	EXPECT_EQ(static_cast<int>('d'), 100);
	EXPECT_EQ(static_cast<int>('h'), 104);
	EXPECT_EQ(static_cast<int>('v'), 118);
	EXPECT_EQ(static_cast<int>('n'), 110);
	EXPECT_EQ(static_cast<int>('x'), 120);
}

TEST(SequenceTest, conversions)
{
	EXPECT_EQ("AGCTAGCT", Sequence<char>(Sequence<CompressedCharTwoInOne>("AGCTagct")).getString());
}
