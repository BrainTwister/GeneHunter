#include "GenomeTest.h"
#include "Gene.h"
#include <set>

using namespace std;
using namespace GeneAssembler;
using boost::filesystem::path;

void GeneTest::operator () ( TestSuite& testSuite ) const
{
	{
		set<Gene> genes;

		genes.insert(Gene("","BruAb1_0704","AAX74079.1","organic solvent tolerance, hypothetical","AE017223.1"));
		genes.insert(Gene("gene","locusTag","proteinID","product","refName"));
		genes.insert(Gene("gene","locusTag","proteinID","product","refName"));
		genes.insert(Gene("gene","locusTag","proteinID2","product","refName"));
		genes.insert(Gene("gene","locusTag","proteinID","product","refName"));
		genes.insert(Gene("","BruAb1_0704","AAX74079.1","organic solvent tolerance, hypothetical","AE017223.1"));

		//for ( auto const& elem : genes ) cout << elem << endl;

		if ( genes.size() != 3 ) testSuite.errorLog(name() + ": genes.size() != 3");
	}
}
