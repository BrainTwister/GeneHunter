#include "GenomeTest.h"
#include "FileIO.h"
#include "Taxonomy.h"
#include <iostream>
#include <string>

using namespace std;
using namespace GeneAssembler;

void TaxonomyTest::operator () ( TestSuite& testSuite ) const
{
	Taxonomy taxonomy;

	if ( taxonomy.getTaxIDByNucGI(24) != 9771 )
		testSuite.errorLog(name() + ": taxonomy.getTaxIDByNucGI(24) failed.");

	try {
		taxonomy.getTaxIDByNucGI(1);
        testSuite.errorLog(name() + ": taxonomy.getTaxIDByNucGI(1) doesn't throw.");
	} catch ( ... ) {}

	if ( taxonomy.getScientificName(2) != "Bacteria" )
		testSuite.errorLog(name() + ": taxonomy.getScientificNameByTaxID(2) failed.");

	if ( taxonomy.getScientificName(633,"genus") != "Yersinia" )
		testSuite.errorLog(name() + ": taxonomy.getScientificNameByTaxID(2) failed.");

	auto taxInfo = taxonomy.getTaxInfo(2);
	if ( get<0>(taxInfo) != 131567 ) testSuite.errorLog(name() + ": taxonomy.getTaxInfo(2) get<0> failed.");
	if ( get<1>(taxInfo) != "superkingdom" ) testSuite.errorLog(name() + ": taxonomy.getTaxInfo(2) get<1> failed.");
	if ( get<2>(taxInfo) != false ) testSuite.errorLog(name() + ": taxonomy.getTaxInfo(2) get<2> failed.");

	if ( taxonomy.getLineage(9606) != "Eukaryota; Metazoa; Chordata; Craniata; Vertebrata; Euteleostomi; Mammalia; Eutheria; Euarchontoglires; Primates; Haplorrhini; Catarrhini; Hominidae; Homo." )
		testSuite.errorLog(name() + ": taxonomy.getLineage(9606) failed.");

	if ( taxonomy.getLineage(633) != "Bacteria; Proteobacteria; Gammaproteobacteria; Enterobacteriales; Enterobacteriaceae; Yersinia." )
		testSuite.errorLog(name() + ": taxonomy.getLineage(633) failed.");
}
