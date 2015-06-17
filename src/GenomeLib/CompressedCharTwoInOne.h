#ifndef COMPRESSEDCHARTWOINONE_H_
#define COMPRESSEDCHARTWOINONE_H_

#include "GeneHunterException.h"
#include "GetCompressedIndex.h"
#include <boost/serialization/access.hpp>
#include "boost/serialization/array.h"
#include "boost/lexical_cast.hpp"
#include <array>
#include <functional>
#include <string>
#include <string>

namespace GeneHunter {

struct CompressedCharTwoInOne
{
    static const size_t CompressionFactor = 2;
    static const size_t size_ = 16;

    CompressedCharTwoInOne()
     : baseItem_(255) // default XX
    {}

    CompressedCharTwoInOne( char c1, char c2 )
     : baseItem_(compress(c1,c2))
    {}

    template < class Iterator >
    CompressedCharTwoInOne( Iterator const& iterBeg, Iterator const& iterEnd )
     : baseItem_(compress(iterBeg,iterEnd))
    {}

    std::string getString() const
    {
        return decompress();
    }

    template < size_t pos >
    char getChar() const
    {
        static_assert( pos < CompressionFactor, "CompressedCharTwoInOne::getChar out of range" );
        return charByCompressionIndex[GetCompressedIndex<size_,CompressionFactor-1,pos>()(baseItem_)];
    }

    template < size_t pos >
    char getCompressedIndex() const
    {
        static_assert( pos < CompressionFactor, "CompressedCharTwoInOne::getCompressedIndex out of range" );
        return GetCompressedIndex<size_,CompressionFactor-1,pos>()(baseItem_);
    }

    bool compareFirst( char c ) const
    {
        return (this->baseItem_ / this->size_) == compressionIndexByChar[c];
    }

    bool compareSecond( char c ) const
    {
        return (this->baseItem_ % this->size_) == compressionIndexByChar[c];
    }

    bool containChar( char c ) const
    {
        return (this->baseItem_ / this->size_) == compressionIndexByChar[c] or
               (this->baseItem_ % this->size_) == compressionIndexByChar[c];
    }

    bool containOnlyPureBases() const
    {
        return (this->baseItem_ / this->size_) < 4 and
               (this->baseItem_ % this->size_) < 4;
    }

    static const char getEndChar()
    {
        return 'X';
    }

    size_t getNbBases() const
    {
        return this->baseItem_ / size_ == 15 ? 0 : this->baseItem_ % size_ == 15 ? 1 : 2;
    }

private:

    template < class T > friend class ::std::hash;
    template < class T > friend class ::std::equal_to;

    friend class boost::serialization::access;

    template < class Archive >
    void serialize( Archive & ar, const unsigned int )
    {
        ar & baseItem_;
    }

    static char compress( char c1, char c2 )
    {
        return compressionIndexByChar[c1] + compressionIndexByChar[c2] * size_;
    }

    template < class Iterator >
    static char compress( Iterator const& iterBeg, Iterator const& iterEnd )
    {
        if ( iterBeg == iterEnd ) return 255;
        else if ( iterBeg+1 == iterEnd ) return compressionIndexByChar[*iterBeg] + 15 * size_;
        else if ( iterBeg+2 == iterEnd ) return compressionIndexByChar[*iterBeg] + compressionIndexByChar[*(iterBeg+1)] * size_;
        else throw GeneHunterException("CompressedCharTwoInOne: error in compress iter");
    }

    std::string decompress() const
    {
        return std::string(1,getChar<0>()) + std::string(1,getChar<1>());
    }

    static const char compressionIndexByChar[256];
    static const char charByCompressionIndex[size_];

    char baseItem_;

};

inline std::ostream& operator << ( std::ostream& os, CompressedCharTwoInOne const& c )
{
    return os << c.getString();
}

} // namespace GeneHunter

namespace std {

template <>
struct hash< GeneHunter::CompressedCharTwoInOne >
{
    size_t operator () ( GeneHunter::CompressedCharTwoInOne const& c ) const
    {
        return hash<char>()(c.baseItem_);
    }
};

template <>
struct equal_to< GeneHunter::CompressedCharTwoInOne >
{
    bool operator () ( GeneHunter::CompressedCharTwoInOne const& c1, GeneHunter::CompressedCharTwoInOne const& c2 ) const
    {
        return equal_to<char>()(c1.baseItem_,c2.baseItem_);
    }
};

} // namespace std

#endif /* COMPRESSEDCHARTWOINONE_H_ */
