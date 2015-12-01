#include "FASTAIterator.h"
#include "GenomeTest.h"

using namespace std;
using namespace GeneAssembler;

void FASTAIteratorTest::operator () ( TestSuite& testSuite ) const
{
	boost::filesystem::path testInput = testSuite.getInputDirectory() / "nt_head";

	for ( FASTAIterator<char> iterCur(testInput), iterEnd; iterCur != iterEnd; ++iterCur )
	{
        //cout << *iterCur << endl;
	}
}
