#include "FASTQIterator.h"
#include "GenomeTest.h"

using namespace std;
using namespace GeneAssembler;

void FASTQIteratorTest::operator () ( TestSuite& testSuite ) const
{
	boost::filesystem::path testInput = testSuite.getInputDirectory() / "Example1_head.fq";

	for ( FASTQIterator iterCur(testInput), iterEnd; iterCur != iterEnd; ++iterCur )
	{
//	    cout << *iterCur << endl;
	}
}
