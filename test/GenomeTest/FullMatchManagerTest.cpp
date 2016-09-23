#include "GenomeLib/FullMatchManager.h"
#include <gtest/gtest.h>

using namespace GeneHunter;

namespace {

struct TestData
{
    TestData(std::vector<FASTA<char>> const& fastaList,
             std::vector<FASTQ> const& fastqList,
             Matches const& matches)
     : fastaList(fastaList),
	   fastqList(fastqList),
	   matches(matches)
    {}

    std::vector<FASTA<char>> fastaList;
    std::vector<FASTQ> fastqList;
    Matches matches;
};

} // namespace anonymous

/// Test fixture
class FullMatchManagerTest
 : public ::testing::WithParamInterface<TestData>,
   public ::testing::Test
{};

TEST_P(FullMatchManagerTest, basic)
{
	NucleotideDatabase<Traits<4, char, char>> db;

	for (auto const& fasta : GetParam().fastaList) {
		db.addEntry(PtrFASTA<char>(new FASTA<char>(fasta)));
	}

	FullMatchManager fmm(FullMatchManagerSettings(TakeMatchWithMostSeeds,100,1,8));

	for (auto const& fastq : GetParam().fastqList) {
		fmm.includeBestMatch(db,fastq);
	}

	EXPECT_EQ(GetParam().matches, fmm.getMatches());
}

INSTANTIATE_TEST_CASE_P(FullMatchManagerTest, FullMatchManagerTest,
    ::testing::Values(
		TestData(
			{{1, "ACGT"}},
			{{"q1", "ACGT", "FFFF"}},
			{{"q1", {1, 0, 4, 3, 2}}}
		),
		TestData(
			{{1, "ACGT"}, {2, "AATCGTGT"}},
			{{"q1", "ACGT", "FFFF"}, {"q2", "GTGT", "FFFF"}},
			{{"q1", {1, 0, 4, 3, 2}}, {"q2", {2, 4, 8, 3, 1}}}
		),
		TestData(
			{{1, "ACGTACGT"},{2, "ACGTACCT"}},
			{{"q1","ACGTACCT","FFFFFFGF"}, {"q2","ACGTACGT","FFFFFFFF"}},
			{{"q2", {1, 0, 8, 7, 4}}, {"q1", {2, 0, 8, 7, 2}}}
		),
		TestData(
			{{1, "ACGTACAT"},{2, "ACGTACGA"}},
			{{"q1", "ACGTACGT", "~~~~~~~#"}, {"q2", "ACGTACTT", "~~~~~~#~"}},
			{{"q1", {2, 0, 8, 6, 2}}, {"q2", {1, 0, 8, 6, 1}}}
		)
    )
);
