#include "GenomeLib/Variant.h"
#include <gtest/gtest.h>

using namespace GeneHunter;

TEST(VariantTest, Default)
{
#if 0
	{
		Variant variant("P1",1212,"--","TA",38,"AGTGTAGATGGCGTAGCCCGATGACACCA");

		path outputFile = testSuite.getOutputDirectory() / "VariantTest.xml";
		std::ofstream output(outputFile.c_str());

		using boost::property_tree::ptree;
		ptree pt;
		variant.writeTree(pt);

		write_xml(output,pt,boost::property_tree::xml_writer_settings<char>(' ',2));
	}
	{
		FullMatchManager fullMatchManager;

		path inputFullMatchManagerFile = testSuite.getInputDirectory() / "GenomeTest" / "FullMatchManager.bin";
		if (!exists(inputFullMatchManagerFile)) throw GeneAssemblerException("File not found: " + inputFullMatchManagerFile.string());

		readBinary(fullMatchManager,"FullMatchManager",inputFullMatchManagerFile);

		cout << "fullMatchManager size = " << fullMatchManager.getMatches().size() << endl;

		multimap< size_t, Range<size_t> > rangeMap;

		for ( auto const& elem : fullMatchManager.getMatches() )
		{
			auto match = elem.second;
			rangeMap.insert(make_pair(match.nuclGI_,Range<size_t>(match.refStart_,match.refEnd_)));
		}

		cout << "rangeMap size = " << rangeMap.size() << endl;

		size_t count(0);
		for ( auto const& elem : rangeMap )
		{
			if ( count == 100 ) break;
			++count;
			cout << elem << endl;
		}
	}
#endif
}
