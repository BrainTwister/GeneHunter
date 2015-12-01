#include "Quality.h"
#include "GenomeTest.h"

using namespace std;
using namespace GeneAssembler;

void QualityTest::operator () ( TestSuite& testSuite ) const
{
	{
		FastqSanger q;

		constexpr std::array<double,FastqSanger::size> arr = FastqSanger::make_arr(FastqSanger::list());

//		std::copy(begin(arr), end(arr), std::ostream_iterator<double>(std::cout, "\n"));
	}
	{
//		for ( char c(33); c != 127; ++c) {
//			cout << c << " " << FastqSanger::data[c] << endl;
//		}
	}
	{
		if ( !testSuite.isEqual(FastqSanger::data[33], 1.0, 1e-4) ) testSuite.errorLog(name() + ": FastqSanger::data[33] failed.");
		if ( !testSuite.isEqual(FastqSanger::data[126], 5.01187e-10, 1e-4) ) testSuite.errorLog(name() + ": FastqSanger::data[126] failed.");
	}
}
