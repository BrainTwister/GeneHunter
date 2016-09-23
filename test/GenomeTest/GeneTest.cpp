#include "GenomeLib/Gene.h"
#include <gtest/gtest.h>
#include <set>
#include <utility>

using namespace GeneHunter;

TEST(GeneTest, default_constructor)
{
	std::set<Gene> genes;

	genes.insert(Gene("","BruAb1_0704","AAX74079.1","organic solvent tolerance, hypothetical","AE017223.1"));
	genes.insert(Gene("gene","locusTag","proteinID","product","refName"));
	genes.insert(Gene("gene","locusTag","proteinID","product","refName"));
	genes.insert(Gene("gene","locusTag","proteinID2","product","refName"));
	genes.insert(Gene("gene","locusTag","proteinID","product","refName"));
	genes.insert(Gene("","BruAb1_0704","AAX74079.1","organic solvent tolerance, hypothetical","AE017223.1"));

	EXPECT_EQ(3, genes.size());
}
