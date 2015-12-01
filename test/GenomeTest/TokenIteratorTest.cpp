#include "GenomeTest.h"
#include "Sequence.h"
#include "TokenIterator.h"
#include <iostream>
#include <string>

using namespace std;
using namespace GeneAssembler;

template < class T, class T2 >
void checkTokenIterator( Sequence<T2> const& seq, vector< pair<string,size_t> > const& tok,
	TestSuite& testSuite, string const& errorMsg )
{
	vector< pair<string,size_t> >::const_iterator iterTokenCur(tok.begin());
	vector< pair<string,size_t> >::const_iterator iterTokenEnd(tok.end());

	T iterCur(seq);
	T iterEnd;

	for ( ; iterCur != iterEnd and iterTokenCur != iterTokenEnd; ++iterCur, ++iterTokenCur )
	{
		//std::cout << iterCur->first.getString() << iterCur->second << std::endl;
		if ( iterCur->first.getString() != iterTokenCur->first or iterCur->second != iterTokenCur->second )
			testSuite.errorLog(errorMsg + "( should be: " + iterTokenCur->first + ", " + boost::lexical_cast<string>(iterTokenCur->second)
				+ " ; is: " + iterCur->first.getString() + ", " + boost::lexical_cast<string>(iterCur->second) + " )");
	}

	for ( ; iterCur != iterEnd; ++iterCur )
	{
		testSuite.errorLog(errorMsg + "( unexpected token: " + iterCur->first.getString() + ", " + boost::lexical_cast<string>(iterCur->second) + " )");
	}

	for ( ; iterTokenCur != iterTokenEnd; ++iterTokenCur )
	{
		testSuite.errorLog(errorMsg + "( missing token: " + iterTokenCur->first + ", " + boost::lexical_cast<string>(iterTokenCur->second) + " )");
	}
}

void TokenIteratorTest::operator () ( TestSuite& testSuite ) const
{
	{
		Sequence<char> s1("GAATTCCAGGAGGACCAGGAG");
						 //012345678901234567890

		//vector< pair<string,size_t> > t1 {{"GAATTCCA",0},{"GGAGGACC",8},{"AGGAGXXX",16}};
		vector< pair<string,size_t> > t1 {{"GAATTCCA",0},{"GGAGGACC",8}};

		checkTokenIterator< TokenIterator<char,8> >(s1,t1,testSuite,name() + ": error 1");
	}
	{
		Sequence<char> s1("GAATTCNCAGGAGGACCAGGAGTAGCC");
						 //012345678901234567890123456

		//vector< pair<string,size_t> > t1 {{"CAGGAGGA",7},{"CCAGGAGT",15},{"AGCCXXXX",23}};
		vector< pair<string,size_t> > t1 {{"CAGGAGGA",7},{"CCAGGAGT",15}};

		checkTokenIterator< TokenIterator<char,8> >(s1,t1,testSuite,name() + ": error 2");
	}
	{
		Sequence<char> s1("GAATTCCA");
						 //01234567

		vector< pair<string,size_t> > t1 {{"GAAT",0},{"TCCA",4}};

		checkTokenIterator< TokenIterator<CompressedCharTwoInOne,4,char> >(s1,t1,testSuite,name() + ": error 3");
	}
	{
		Sequence<CompressedCharTwoInOne> s1("GAATTCAGTAGTAGCC");
										   //0123456789012345

		vector< pair<string,size_t> > t1 {{"GAATTCAG",0},{"TAGTAGCC",8}};

		checkTokenIterator< TokenIterator<CompressedCharTwoInOne,8> >(s1,t1,testSuite,name() + ": error 4");
	}
	{
		Sequence<CompressedCharTwoInOne> s1("GAATTCNCAGGAGGACCAGGAGTAGCC");
										   //012345678901234567890123456

		//vector< pair<string,size_t> > t1 {{"AGGAGGAC",8},{"CAGGAGTA",16},{"GCCXXXXX",24}};
		vector< pair<string,size_t> > t1 {{"AGGAGGAC",8},{"CAGGAGTA",16}};

		checkTokenIterator< TokenIterator<CompressedCharTwoInOne,8> >(s1,t1,testSuite,name() + ": error 5");
	}
	{
		Sequence<char> s1("GACTTNCCAGTT");
						 //012345678901

		//vector< pair<string,size_t> > t1 {{"CCAGTTXX",6}};
		vector< pair<string,size_t> > t1 {};

		checkTokenIterator< TokenIterator<char,8> >(s1,t1,testSuite,name() + ": error 6");
	}
	{
		Sequence<char> s1("GACTC");

		vector< pair<string,size_t> > t1 {{"GACT",0},{"ACTC",1}};

		checkTokenIterator< TokenIterator<char,4,char,1> >(s1,t1,testSuite,name() + ": error 7");
	}
	{
		Sequence<CompressedCharTwoInOne> s1("GACTCTAG");

		vector< pair<string,size_t> > t1 {{"GACT",0},{"CTAG",4}};

		checkTokenIterator< TokenIterator<CompressedCharFourInOne,4,CompressedCharTwoInOne> >(s1,t1,testSuite,name() + ": error 8");
	}
	{
		Sequence<CompressedCharTwoInOne> s1("GACTCTAGGAATTCCAGGAGGACCAGGAGTAGCC");
										   //0123456789012345678901234567890123

		vector< pair<string,size_t> > t1 {{"GACTCTAGGAATTCCAGGAGGACC",0}};

		checkTokenIterator< TokenIterator<CompressedCharFourInOne,24,CompressedCharTwoInOne> >(s1,t1,testSuite,name() + ": error 9");
	}
#if 0
	{
		Sequence<char> s1("GACT");
						 //0123

		vector< pair<string,size_t> > t1 {{"GACTXX",0}};

		checkTokenIterator< TokenIterator<char,4,char> >(s1,t1,testSuite,name() + ": error 10");
	}
#endif
}
