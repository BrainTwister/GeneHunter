#include "GeneAssemblerException.h"
#include "Settings.h"
#include <string>

namespace GeneAssembler {

std::ostream& operator << ( std::ostream& os, SearchAlgorithm s )
{
	switch (s) {
	    case FirstTokensAsSeed :
	    	return os << "FirstTokensAsSeed";
	    case TakeMatchWithMostSeeds :
	    	return os << "TakeMatchWithMostSeeds";
	    default:
	    	throw GeneAssemblerException("FullMatchManager: unkown searchAlgorithm");
	}
}

std::istream& operator >> ( std::istream& is, SearchAlgorithm& s )
{
	std::string str;
	is >> str;
	if ( str == "FirstTokensAsSeed" )
	    s = FirstTokensAsSeed;
	else if ( str == "TakeMatchWithMostSeeds" )
	    s = TakeMatchWithMostSeeds;
	else
	   	throw GeneAssemblerException("FullMatchManager: unkown searchAlgorithm");
	return is;
}

} // namespace GeneAssembler
