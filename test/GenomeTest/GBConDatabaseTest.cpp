#include "FileIO.h"
#include "GenomeTest.h"
#include "CDSDatabase.h"
#include <iostream>

using namespace std;
using namespace GeneAssembler;

void CDSDatabaseTest::operator () ( TestSuite& testSuite ) const
{
	CDSDatabase::Settings cdsDatabaseSettings;
	readXML(cdsDatabaseSettings, "CDSDatabaseSettings", testSuite.getInputDirectory()/"CDSDatabaseSettings.xml");

	CDSDatabase cdsDatabase(cdsDatabaseSettings);

	auto genes = cdsDatabase.getGene(48994873,4531605,4531753);

	if (genes.empty()) testSuite.errorLog(name() + ": gene not found.");

    if (genes[0].product != "KpLE2 phage-like element; predictedacetyltransferase")
		testSuite.errorLog(name() + ": wrong gene product name.");
}
