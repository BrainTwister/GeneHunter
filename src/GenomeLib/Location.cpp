#include "GeneAssemblerException.h"
#include "Location.h"
#include <boost/tokenizer.hpp>
#include <iostream>

using namespace std;

namespace GeneAssembler {

std::set<char> Location::charSet_ = {'0','1','2','3','4','5','6','7','8','9','<','>','.'};

Location::Location() {}

Location::Location( std::string const& locationString )
{
	bool lastCharWasNonRangeChar = true;
	vector<string> rawRangeList;
	vector<string>::iterator iterRawRange;

    for ( auto const& character : locationString ) {
    	if ( charSet_.find(character) != charSet_.end() ) {
    		if ( lastCharWasNonRangeChar ) {
    			rawRangeList.push_back("");
    			iterRawRange = rawRangeList.end()-1;
    			lastCharWasNonRangeChar = false;
    		}
    		*iterRawRange += character;
    	}
    	else lastCharWasNonRangeChar = true;
    }

    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;

    string firstString,secondString;
    for ( auto const& rawRange : rawRangeList )
    {
    	RangeType range;
    	boost::char_separator<char> sep(".");
	    tokenizer token(rawRange,sep);
	    tokenizer::const_iterator iterToken = token.begin();

	    if ( iterToken == token.end() )
	    	throw GeneAssemblerException("Location: error reading " + locationString);

	    firstString = *iterToken;
	    ++iterToken;

	    // Single base location
	    if ( iterToken == token.end() )
	    	secondString = firstString;
	    else {
			secondString = *iterToken;
			++iterToken;
	    }

	    if ( iterToken != token.end() )
	    	throw GeneAssemblerException("Location: error reading " + locationString);

        if ( firstString[0] == '<' ) {
        	range.setOpenLowerBoundary(true);
        	firstString.erase(0,1);
        }
        range.setLowerBoundary(firstString);

        if ( secondString[0] == '>' ) {
        	range.setOpenUpperBoundary(true);
        	secondString.erase(0,1);
        }
        range.setUpperBoundary(secondString);

        rangeSet_.insert(range);
    }
}

Location::Location( Range<uint32_t> const& range )
{
	rangeSet_.insert(range);
}

bool Location::within( size_t first, size_t last ) const
{
	for ( auto const& range : rangeSet_ ) {
		if ( first > range.getUpperBoundary() ) continue;
		if ( last < range.getLowerBoundary() ) return false;
        if ( first > range.getLowerBoundary() and last < range.getUpperBoundary() ) return true;
        if ( range.getOpenLowerBoundary() and last > range.getLowerBoundary() and last < range.getUpperBoundary() ) return true;
        if ( range.getOpenUpperBoundary() and first < range.getUpperBoundary() and first > range.getLowerBoundary() ) return true;
	}
	return false;
}

bool Location::match( size_t first, size_t last ) const
{
	for ( auto const& range : rangeSet_ ) {
		if ( last >= range.getLowerBoundary() and first <= range.getUpperBoundary() ) return true;
	}
	return false;
}

Range<uint32_t> Location::getEnclosingRange() const
{
	if (rangeSet_.empty()) return Range<uint32_t>();

	auto iterBeg = rangeSet_.begin();
	auto iterLast = --rangeSet_.end();
	return Range<uint32_t>(iterBeg->getLowerBoundary(),iterLast->getUpperBoundary(),
		iterBeg->getOpenLowerBoundary(),iterLast->getOpenUpperBoundary());
}

std::ostream& operator << ( std::ostream& os, Location const& location )
{
	auto &last = *(--location.rangeSet_.end());

	for ( auto const& range : location.rangeSet_ ) {
		os << range << (&range != &last ? "," : "");
	}

	return os;
}

} // namespace GeneAssembler
