#ifndef RANGE_H_
#define RANGE_H_

#include <boost/lexical_cast.hpp>
#include <boost/serialization/access.hpp>
#include <iostream>
#include <string>

namespace GeneAssembler {

template < class T >
struct Range
{
	typedef T IndexType;

	Range( T lowerBoundary = 0, T upperBoundary = 0, bool openLowerBoundary = false, bool openUpperBoundary = false )
	 : lowerBoundary_(lowerBoundary),
	   upperBoundary_(upperBoundary),
	   openLowerBoundary_(openLowerBoundary),
	   openUpperBoundary_(openUpperBoundary)
	{}

	T getLowerBoundary() const { return lowerBoundary_; }
	T getUpperBoundary() const { return upperBoundary_; }
	bool getOpenLowerBoundary() const { return openLowerBoundary_; }
	bool getOpenUpperBoundary() const { return openUpperBoundary_; }

	void setLowerBoundary( T lowerBoundary ) { lowerBoundary_ = lowerBoundary; }
	void setUpperBoundary( T upperBoundary ) { upperBoundary_ = upperBoundary; }
	void setOpenLowerBoundary( bool openLowerBoundary ) { openLowerBoundary_ = openLowerBoundary; }
	void setOpenUpperBoundary( bool openUpperBoundary ) { openUpperBoundary_ = openUpperBoundary; }

	void setLowerBoundary( std::string const& lowerBoundaryString ) { lowerBoundary_ = boost::lexical_cast<T>(lowerBoundaryString); }
	void setUpperBoundary( std::string const& upperBoundaryString ) { upperBoundary_ = boost::lexical_cast<T>(upperBoundaryString); }

	bool operator == ( Range const& other ) const {
		return lowerBoundary_ == other.lowerBoundary_ and upperBoundary_ == other.upperBoundary_ and
			   openLowerBoundary_ == other.openLowerBoundary_ and openUpperBoundary_ == other.openUpperBoundary_;
	}

	bool operator != ( Range const& other ) const {
		return !operator==(other);
	}

	bool operator < ( Range const& other ) const {
		return lowerBoundary_ < other.lowerBoundary_ or openLowerBoundary_ or openUpperBoundary_;
	}

private:

	template < class T2 >
	friend std::ostream& operator << ( std::ostream& os, Range<T2> const& range );

    friend class boost::serialization::access;

    template < class Archive >
    void serialize( Archive & ar, const unsigned int )
    {
        ar & lowerBoundary_;
        ar & upperBoundary_;
        ar & openLowerBoundary_;
        ar & openUpperBoundary_;
    }

	T lowerBoundary_;
	T upperBoundary_;
	bool openLowerBoundary_ = false;
	bool openUpperBoundary_ = false;

};

template < class T2 >
std::ostream& operator << ( std::ostream& os, Range<T2> const& range )
{
	return os << (range.openLowerBoundary_ ? "<" : "")
		      << boost::lexical_cast<std::string>(range.lowerBoundary_)
		      << ".."
		      << (range.openUpperBoundary_ ? ">" : "")
		      << boost::lexical_cast<std::string>(range.upperBoundary_);
}

} // namespace GeneAssembler

#endif /* RANGE_H_ */
