#ifndef SEQUENCE_H_
#define SEQUENCE_H_

#include "CharTypes.h"
#include "GeneHunterException.h"
#include <boost/serialization/access.hpp>
#include <boost/serialization/string.hpp>
#include <algorithm>
#include <string>
#include <vector>

namespace GeneHunter {

template < class T >
struct Sequence
{
	typedef T Type;
	typedef Sequence<Type> Self;
	typedef std::vector<Type> ContainerType;
	typedef typename ContainerType::const_iterator ConstIterator;

	static const size_t CompressionFactor = GetNbBaseItemsInChar<Type>::value;

	Sequence() = default;

	Sequence( Self const& other ) = default;

	Sequence( std::string const& seqString )
	 : container_(seqString.size() / CompressionFactor + ((seqString.size() % CompressionFactor) == 0 ? 0 : 1))
    {
		size_t compressedSize = seqString.size() / CompressionFactor;
		size_t i(0);
		std::string::const_iterator iterCur(seqString.begin());
		for ( ; i != compressedSize; ++i, iterCur += CompressionFactor )
		{
		    container_[i] = Type(iterCur, iterCur + CompressionFactor );
		}
		if ( seqString.size() % CompressionFactor )
		{
		    container_[i] = Type(iterCur, iterCur + seqString.size() % CompressionFactor);
		}
    }

	Self& operator = ( Self const& other ) = default;

	T& operator [] ( size_t i )       { return container_[i]; }
	T  operator [] ( size_t i ) const { return container_[i]; }

	ConstIterator begin() const { return container_.begin(); }
	ConstIterator end()   const { return container_.end(); }

	size_t size() const { return container_.size(); }

	size_t getNbBases() const { return container_.size() ? (container_.size()-1) * CompressionFactor + container_[container_.size()-1].getNbBases() : 0; }

    std::string getString() const {
    	std::string result;
		for ( auto const& elem : container_ ) result += elem.getString();
		return result;
	}

private:

    friend class boost::serialization::access;

    template < class Archive >
    void serialize( Archive & ar, const unsigned int version )
    {
        ar & container_;
    }

    ContainerType container_;

};

template < size_t i, size_t size >
struct StaticFor {
	template < class Iterator, class CharType >
	void operator () ( Iterator& iter, CharType const& elem ) {
        iter[i] = elem.template getChar<i>();
		StaticFor<i+1,size>()(iter,elem);
	}
};

template < size_t size >
struct StaticFor<size,size> {
	template < class Iterator, class CharType >
	void operator () ( Iterator& iter, CharType const& elem ) {}
};

template < >
struct Sequence<char>
{
	typedef char Type;
	typedef Sequence<Type> Self;
	typedef std::vector<Type> ContainerType;
	typedef ContainerType::const_iterator ConstIterator;

	Sequence() = default;

	Sequence( std::string const& rhs )
	 : container_(rhs.size())
    {
		std::transform(rhs.cbegin(),rhs.cend(),container_.begin(),[]( char c ){ return toupper(c); });
    }

	Sequence( Self const& other ) = default;

	template < class T >
	Sequence( Sequence<T> const& other )
	 : container_(other.size() * T::CompressionFactor)
    {
		ContainerType::iterator iterCur = container_.begin();
		for ( auto const& elem : other ) {
			StaticFor<0,T::CompressionFactor>()(iterCur,elem);
			iterCur += T::CompressionFactor;
		}
    }

	Self& operator = ( Self const& other ) = default;

	char& operator [] ( size_t i )       { return container_[i]; }
	char  operator [] ( size_t i ) const { return container_[i]; }

	ConstIterator begin() const { return container_.begin(); }

	ConstIterator end() const { return container_.end(); }

	size_t size() const { return container_.size(); }

	size_t getNbBases() const { return container_.size(); }

    std::string getString() const {
    	std::string result;
		for ( auto const& elem : container_ ) result += elem;
		return result;
	}

private:

    friend class boost::serialization::access;

    template < class Archive >
    void serialize( Archive & ar, const unsigned int version )
    {
        ar & container_;
    }

    ContainerType container_;

};

template < class T >
Sequence<T> reverse( Sequence<T> const& seq ) {
	std::string ss = seq.getString();
	return Sequence<T>(std::string(ss.rbegin(),ss.rend()));
}

template < class T >
Sequence<T> complement( Sequence<T> const& seq ) {
	std::string s1 = seq.getString();
	std::string s2;
	for ( auto const& c : s1 ) {
        if      ( c == 'A' ) s2 += 'T';
        else if ( c == 'T' ) s2 += 'A';
        else if ( c == 'G' ) s2 += 'C';
        else if ( c == 'C' ) s2 += 'G';
        else if ( c == 'K' ) s2 += 'M';
        else if ( c == 'S' ) s2 += 'W';
        else if ( c == 'R' ) s2 += 'Y';
        else if ( c == 'M' ) s2 += 'K';
        else if ( c == 'W' ) s2 += 'S';
        else if ( c == 'Y' ) s2 += 'R';
        else if ( c == 'B' ) s2 += 'V';
        else if ( c == 'D' ) s2 += 'H';
        else if ( c == 'H' ) s2 += 'D';
        else if ( c == 'V' ) s2 += 'B';
        else if ( c == 'N' ) s2 += 'X';
        else if ( c == 'X' ) s2 += 'N';
        else if ( c == 'U' ) s2 += 'A';
        else throw GeneHunterException("complement: unsupported char " + std::string(1,c));
	}
	return Sequence<T>(s2);
}

template < class T >
Sequence<T> reverseComplement( Sequence<T> const& seq ) {
	return reverse(complement(seq));
}

template < class T >
inline std::ostream& operator << ( std::ostream& os, Sequence<T> const& seq )
{
	return os << seq.getString();
}

} // namespace GeneHunter

#endif /* SEQUENCE_H_ */
