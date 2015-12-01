#include "CDSDatabase.h"
#include "GenomeTest.h"
#include "FileIO.h"
#include "OrganismsReport.h"
#include <string>

using namespace std;
using namespace GeneAssembler;

void OrganismsReportTest::operator () ( TestSuite& testSuite ) const
{
	OrganismsReport report( testSuite.getOutputDirectory() / "OrganismsReportTest.xml",
		testSuite.getInputDirectory() / "Example1_description.xml", OrganismsReport::Settings(), 0 );

	Matches matches;
	matches.insert(make_pair("read1",Match(48994873,4531605,4531753,739,1)));

	report.addMatches(matches);
}
