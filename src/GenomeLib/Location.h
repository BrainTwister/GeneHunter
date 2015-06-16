#ifndef LOCATION_H_
#define LOCATION_H_

#include "Range.h"
#include <boost/lexical_cast.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/set.hpp>
#include <set>
#include <string>
#include <vector>

namespace GeneHunter {

/**
 * Interpret and manage a CDS location.
 */
class Location
{
public:

	Location();

	Location( std::string const& locationString );

	Location( Range<uint32_t> const& range );

    bool operator < ( Location const& other ) const
    {
    	return rangeSet_.begin()->getLowerBoundary() < other.rangeSet_.begin()->getLowerBoundary();
    }

    /// Returns true if the given range is totally within the location.
    bool within( size_t first, size_t last ) const;

    /// Returns true if at least one base is within the location.
    bool match( size_t first, size_t last ) const;

    Range<uint32_t> getEnclosingRange() const;

private:

	friend std::ostream& operator << ( std::ostream& os, Location const& location );

    friend class boost::serialization::access;

    template < class Archive >
    void serialize( Archive & ar, const unsigned int )
    {
        ar & rangeSet_;
    }

    typedef Range<uint32_t> RangeType;

    typedef std::set<RangeType> RangeSet;

    RangeSet rangeSet_;

    static std::set<char> charSet_;

};

std::ostream& operator << ( std::ostream& os, Location const& location );

} // namespace GeneHunter

#endif /* LOCATION_H_ */
