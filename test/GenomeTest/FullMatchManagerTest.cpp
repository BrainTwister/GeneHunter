#include "FullMatchManager.h"
#include "GenomeTest.h"
#include "ValueWithStdDeviation.h"
#include <boost/filesystem/path.hpp>

using namespace std;
using Utilities::ValueWithStdDeviation;
using namespace GeneAssembler;

struct MatchHelper
{
	MatchHelper( std::string const& name, size_t geneID = 0, size_t refStart = 0,
		size_t refEnd = 0, QualityType quality = 0.0, size_t nbTokens = 0
	) : name(name), match(geneID,refStart,refEnd,quality,nbTokens)
	{}

	std::string name;
    Match match;
};

template < class Traits >
struct Check
{
	typedef typename Traits::RefSeqCharType RefSeqCharType;

	Check( TestSuite& testSuite, string name )
	 : testSuite(testSuite), name(name)
	{}

	void operator () ( size_t num,
		std::initializer_list<FASTA<RefSeqCharType> > fastaList,
		std::initializer_list<FASTQ> fastqList,
		std::initializer_list<MatchHelper> matchList )
	{
		NucleotideDatabase<Traits> db;

		for ( auto const& fasta : fastaList ) {
			db.addEntry(PtrFASTA<RefSeqCharType>(new FASTA<RefSeqCharType>(fasta)));
		}

		//db.printHashMapReferenceSequenceTokens();

		FullMatchManager fmm(FullMatchManagerSettings(TakeMatchWithMostSeeds,100,1,8));

		for ( auto const& fastq : fastqList ) {
			fmm.includeBestMatch(db,fastq);
		}

		Matches checkMatches;

		for ( auto const& match : matchList ) {
			checkMatches.insert(std::make_pair(match.name,match.match));
		}

		if ( fmm.getMatches() != checkMatches ) {
			string testName = name + " " + boost::lexical_cast<string>(num);
			cerr << "Error information for " << testName  << endl;
			cerr << fmm.getMatches() << endl;
			cerr << "End error information for " << testName << endl;
			testSuite.errorLog(testName);
		}
	}

private:

	TestSuite& testSuite;
	string name;

};

void FullMatchManagerTest::operator () ( TestSuite& testSuite ) const
{
	typedef Traits<4> TraitsType;

	Check<TraitsType> check(testSuite,name());

	check( 1,
		{{1,"ACGT"}},
		{{"q1","ACGT","FFFF"}},
		{{"q1",1,0,4,3,2}}
	);

	check( 2,
		{{1,"ACGT"}, {2,"AATCGTGT"}},
		{{"q1","ACGT","FFFF"}, {"q2","GTGT","FFFF"}},
		{{"q1",1,0,4,3,2}, {"q2",2,4,8,3,1}}
	);

//	check( 3,
//		{{1,"ACGT"}, {2,"AATCGT"}},
//		{{"q1","ACGT","FFFF"}, {"q2","TCGT","FFFF"}},
//		{{"q1",1,0,4,1.0}, {"q2",2,2,6,1.0}}
//	);

	check( 4,
		{{1,"ACGTACGT"},{2,"ACGTACCT"}},
		{{"q1","ACGTACCT","FFFFFFGF"}, {"q2","ACGTACGT","FFFFFFFF"}},
		{{"q2",1,0,8,7,4}, {"q1",2,0,8,7,2}}
	);

	check( 5,
		{{1,"ACGTACAT"},{2,"ACGTACGA"}},
		{{"q1","ACGTACGT","~~~~~~~#"}, {"q2","ACGTACTT","~~~~~~#~"}},
		{{"q1",2,0,8,6,2}, {"q2",1,0,8,6,1}}
	);
}
