#include "FASTQIterator.h"
#include "GenomeTest.h"
#include "Sequence.h"
#include "LocalAlignment.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;
using namespace GeneAssembler;

void LocalAlignmentTest::operator () ( TestSuite& testSuite ) const
{
	{
		LocalAlignment localAlignment;

		Sequence<char> seq1("AGCTAGCT");
		Sequence<char> seq2("AGCTTAGCT");
		Sequence<char> quality("~~~~~~~~");

		auto result = localAlignment.getQuality(seq1,seq2,quality);
		if ( !testSuite.isEqual(result,7.0,1e-4) ) testSuite.errorLog(name() + ": test 1 failed.");
	}
	{
		LocalAlignment localAlignment;

		Sequence<char> seq1("AGCTCCC");
		Sequence<char> seq2("AGCTGGG");
		Sequence<char> quality("~~~~~~~~");

		auto result = localAlignment.getQuality(seq1,seq2,quality);
		if ( !testSuite.isEqual(result,4.0,1e-4) ) testSuite.errorLog(name() + ": test 2 failed.");
	}
	{
		LocalAlignment localAlignment;

		Sequence<char> seq1("AGCTTCC");
		Sequence<char> seq2("AGCTGTC");
		Sequence<char> quality("~~~~~~~");

		auto result = localAlignment.getQuality(seq1,seq2,quality);
		if ( !testSuite.isEqual(result,5.0,1e-4) ) testSuite.errorLog(name() + ": test 3 failed.");
	}
	{
		LocalAlignment localAlignment;

		boost::filesystem::path file = testSuite.getInputDirectory()/"GenomeTest"/"read1.fq";

		FASTQIterator iterCur(file);
		FASTQ fastq = **iterCur;

		Sequence<char> read = fastq.getSequence();
		Sequence<char> quality = fastq.getQuality();

		Sequence<char> match1("CCATTTCCATGAACTACTGAAACTGTATGTCCTTCTTCAATTAAATCTACTATAGATTTTGCTGTATCCTTACAAGTCTTTAATTGACTTTCTGGAGTTTTGTCTTTTGGATCTGATTGTAAGGCATTTCCTCCTAAAGCTAATACTATTTTCATAAAAGCACCTCCGAAAAATAATATGTTTAATACTTTAAATTTAATCACATA");
		Sequence<char> match2("GTTGTAGTGAGCGGGCTTGCCCCGCGCTGGGCTGCGAAGCGGCCCTAATCCGGCCAACTCGTTTTACCTGAAAGAACGCGGTGGCCGTGTTGGGGCTGCTTCGCAGCCCAGCGCGGGGCAAGCCCGCTCACCACAAAAAAGCCCGCTCACCACAGCAAGCCCGCTCACCACAAGGTCCCGCCAGCCACAGGCGTAAGGTGTTTTCA");

		auto result1 = localAlignment.getQuality(read,match1,quality);
		if ( !testSuite.isEqual(result1,134.426,1e-3) ) testSuite.errorLog(name() + ": test 4 failed.");

		auto result2 = localAlignment.getQuality(read,match2,quality);
		if ( !testSuite.isEqual(result2,10.5979,1e-3) ) testSuite.errorLog(name() + ": test 5 failed.");
	}
	{
		LocalAlignment localAlignment(LocalAlignment::Settings(0,true,2));

		Sequence<char> seq1("AGCTTCC");
		Sequence<char> seq2("AGCTGTC");
		Sequence<char> quality("~~~~~~~");

		auto result = localAlignment.getQuality(seq1,seq2,quality);
		if ( !testSuite.isEqual(result,5.0,1e-4) ) testSuite.errorLog(name() + ": test 6 failed.");
	}
	{
		LocalAlignment localAlignment(LocalAlignment::Settings(0,true,2));

		Sequence<char> seq1("AGCTTCC");
		Sequence<char> seq2("AGCTACGTCG");
		Sequence<char> quality("~~~~~~~");

		auto result = localAlignment.getQuality(seq1,seq2,quality);
		if ( !testSuite.isEqual(result,4.0,1e-4) ) testSuite.errorLog(name() + ": test 7 failed.");
	}
	{
		LocalAlignment localAlignment(LocalAlignment::Settings(0,true,50));

		boost::filesystem::path file = testSuite.getInputDirectory()/"GenomeTest"/"read2.fq";

		FASTQIterator iterCur(file);
		FASTQ fastq = **iterCur;

		Sequence<char> read = fastq.getSequence();
		Sequence<char> quality = fastq.getQuality();

		boost::filesystem::path refFile = testSuite.getInputDirectory()/"GenomeTest"/"ref2.fa";

		ifstream infile(refFile.c_str());
		string sequence;
		infile >> sequence;

		Sequence<char> match(sequence);

		auto result = localAlignment.getQuality(read,match,quality);
		if ( !testSuite.isEqual(result,325.07,1e-3) ) testSuite.errorLog(name() + ": test 8 failed.");
	}
}
