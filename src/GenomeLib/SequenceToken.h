#ifndef SEQUENCETOKEN_H_
#define SEQUENCETOKEN_H_

#include "GenomeLib/CharTypes.h"
#include "GenomeLib/Sequence.h"
#include "UtilitiesLib/GeneHunterException.h"
#include "boost/lexical_cast.hpp"
#include "boost/mpl/bool.hpp"
#include "BoostLib/boost/serialization/path.h"
#include <boost/type_traits.hpp>
#include <array>
#include <cassert>
#include <functional>
#include <string>

namespace GeneHunter {

template < class T, size_t N >
struct SequenceToken
{
    typedef T Type;
    typedef SequenceToken<Type,N> Self;

    static const size_t Size = N;
    static const size_t CompressionFactor = GetNbBaseItemsInChar<T>::value;
    static const size_t CompressedSize = N / CompressionFactor;

    typedef std::array<T,CompressedSize> ContainerType;

    SequenceToken()
     : token_()
    {}

    template < class Iterator >
    SequenceToken( Iterator const& beg, Iterator const& end,
        typename boost::enable_if< boost::is_same<typename Iterator::value_type,T> >::type* = 0 )
    {
        assert(std::distance(beg, end) == CompressedSize);
        std::fill( std::copy(beg,end,token_.begin()), token_.end(), Type() );
    }

    template < class Iterator >
    SequenceToken( Iterator const& beg, Iterator const& end,
        typename boost::disable_if< boost::is_same<typename Iterator::value_type,T> >::type* = 0 )
    {
        typedef typename Iterator::value_type CharTypeInSeq;
        static const size_t CompressionFactorInSeq = GetNbBaseItemsInChar<CharTypeInSeq>::value;
        static const size_t CompressedSizeInSeq = N / CompressionFactorInSeq;
        static const size_t FactorInOut = CompressionFactor / CompressionFactorInSeq;

        assert(std::distance(beg, end) == CompressedSizeInSeq);

        Iterator iterCur(beg);
        Iterator iterEnd = std::distance(beg,end) > FactorInOut ? beg + FactorInOut : end;
        for ( size_t i(0); i != CompressedSize; ++i, iterCur += FactorInOut,
            iterEnd = std::distance(iterCur,end) > FactorInOut ? iterCur + FactorInOut : end )
        {
            token_[i] = Type(iterCur,iterEnd);
        }
    }

    SequenceToken( std::string const& rhs )
     : SequenceToken{ rhs.begin(), rhs.end() }
    {}

    std::string getString() const {
        std::string result;
        for ( auto const& elem : token_ ) result += elem.getString();
        return result;
    }

private:

    friend class boost::serialization::access;

    template < class Archive >
    void serialize( Archive & ar, const unsigned int )
    {
        ar & token_;
    }

    template < class T2, size_t N2 >
    friend class std::hash;

    template < class T2, size_t N2 >
    friend class std::equal_to;

    ContainerType token_;

};

template < size_t N >
struct SequenceToken<char,N>
{
    typedef SequenceToken<char,N> Self;
    typedef char Type;
    typedef std::array<char,N> ContainerType;

    static const size_t Size = N;

    SequenceToken()
     : token_()
    {}

    SequenceToken( std::string::const_iterator const& beg, std::string::const_iterator const& end )
     : token_()
    {
        assert (std::distance(beg, end) <= N);
        std::fill( std::copy(beg,end,token_.begin()), token_.end(), 'X' );
    }

    SequenceToken( Sequence<char>::ConstIterator const& beg, Sequence<char>::ConstIterator const& end )
     : token_()
    {
        assert(std::distance(beg, end) <= N);
        std::fill( std::copy(beg,end,token_.begin()), token_.end(), 'X' );
    }

    SequenceToken( std::string const& otherToken )
     : token_()
    {
        std::copy(otherToken.begin(),otherToken.end(),token_.begin());
    }

    SequenceToken( std::array<char,N> const& otherToken )
     : token_(otherToken)
    {}

    SequenceToken( Self const& other )
     : token_(other.token_)
    {}

    SequenceToken& operator = ( Self const& other )
    {
        if ( &other != this ) {
            token_ = other.token_;
        }
        return *this;
    }

    std::string getString() const {
        std::string result;
        for ( auto const& elem: token_ ) result += elem;
        return result;
//        std::string result;
//        result.reserve(N);
//        std::copy(token_.begin(),token_.end(),result.begin());
//        return result;
    }

private:

    friend class boost::serialization::access;

    template < class Archive >
    void serialize( Archive & ar, const unsigned int )
    {
        ar & token_;
    }

    template < class T2, size_t N2 >
    friend class std::hash;

    template < class T2, size_t N2 >
    friend class std::equal_to;

    std::array<char,N> token_;

};

template < class T, size_t N >
SequenceToken<T,N> reverse( SequenceToken<T,N> const& seq ) {
    std::string ss = seq.getString();
    return SequenceToken<T,N>(std::string(ss.rbegin(),ss.rend()));
}

template < class T, size_t N >
SequenceToken<T,N> complement( SequenceToken<T,N> const& seq ) {
    std::string s1 = seq.getString();
    std::string s2;
    for ( auto & c : s1 ) {
        if      ( c == 'A' ) s2 += 'T';
        else if ( c == 'T' ) s2 += 'A';
        else if ( c == 'G' ) s2 += 'C';
        else if ( c == 'C' ) s2 += 'G';
        else throw GeneHunterException("complement: wrong char " + c);
    }
    return SequenceToken<T,N>(s2);
}

template < class T, size_t N >
SequenceToken<T,N> reverseComplement( SequenceToken<T,N> const& seq ) {
    return reverse(complement(seq));
}

template < class T, size_t N >
inline std::ostream& operator << ( std::ostream& os, SequenceToken<T,N> const& token )
{
    return os << token.getString();
}

} // namespace GeneHunter

namespace std {

template < class T >
inline void hash_combine( size_t& seed, T const& t )
{
    seed ^= hash<T>()(t) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

template < class T >
inline size_t hash_range( T iterCur, T const& iterEnd )
{
    size_t seed(0);
    for ( ; iterCur != iterEnd; ++iterCur )
    {
        std::hash_combine(seed,*iterCur);
    }
    return seed;
}

template < class T, size_t N >
struct hash< GeneHunter::SequenceToken<T,N> >
{
    size_t operator () ( GeneHunter::SequenceToken<T,N> const& t ) const
    {
        return std::hash_range(t.token_.begin(),t.token_.end());
    }
};

template < class T, size_t N >
struct equal_to< GeneHunter::SequenceToken<T,N> >
{
    bool operator () ( GeneHunter::SequenceToken<T,N> const& t1, GeneHunter::SequenceToken<T,N> const& t2 ) const
    {
        for ( size_t i(0); i != N / GeneHunter::GetNbBaseItemsInChar<T>::value; ++i )
        {
            if ( !equal_to<T>()(t1.token_[i],t2.token_[i]) ) return false;
        }
        return true;
    }
};

} // namespace std

#endif /* SEQUENCETOKEN_H_ */
