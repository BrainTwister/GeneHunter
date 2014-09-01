#ifndef COMPRESSEDCHARFOURINONE_H_
#define COMPRESSEDCHARFOURINONE_H_

#include "CompressedCharTwoInOne.h"
#include "GeneAssemblerException.h"
#include "GetCompressedIndex.h"
#include "Power.h"
#include <boost/lexical_cast.hpp>
#include <boost/serialization/access.hpp>
#include "boost/serialization/array.h"
#include <boost/type_traits.hpp>
#include <array>
#include <functional>
#include <string>

namespace GeneAssembler {

struct CompressedCharFourInOne
{
	static const size_t CompressionFactor = 4;
	static const size_t size_ = 4;

	CompressedCharFourInOne()
	 : baseItem_(0) // default AAAA
	{}

	CompressedCharFourInOne( char c1, char c2, char c3, char c4 )
	 : baseItem_(compress(c1,c2,c3,c4))
	{}

	template < class Iterator >
	CompressedCharFourInOne( Iterator const& iterBeg, Iterator const& iterEnd )
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

	bool containChar( char c ) const
	{
		return (this->baseItem_ / Power<this->size_,3>::value) == compressionIndexByChar[c] or
			   (this->baseItem_ / Power<this->size_,2>::value) == compressionIndexByChar[c] or
		       (this->baseItem_ / this->size_) == compressionIndexByChar[c] or
			   (this->baseItem_ % this->size_) == compressionIndexByChar[c];
	}

	bool containOnlyPureBases() const
	{
		return true;
	}

    static const char getEndChar()
    {
    	return 'A';
    }

    size_t getNbBases() const
    {
    	throw GeneAssemblerException("CompressedCharFourInOne: number of bases are unkown.");
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

	char compress( char c1, char c2, char c3, char c4 ) const
	{
		return compressionIndexByChar[c1]
		     + compressionIndexByChar[c2] * size_
			 + compressionIndexByChar[c3] * Power<size_,2>::value
			 + compressionIndexByChar[c4] * Power<size_,3>::value;
	}

	template < class Iterator >
	char compress( Iterator const& iterBeg,	Iterator const& iterEnd,
		typename boost::enable_if< boost::is_same<typename Iterator::value_type,char> >::type* = 0 ) const
	{
		if ( iterBeg == iterEnd ) return 0;
		else if ( iterBeg+1 == iterEnd ) return compressionIndexByChar[*iterBeg];
		else if ( iterBeg+2 == iterEnd ) return compressionIndexByChar[*iterBeg]
		                                      + compressionIndexByChar[*(iterBeg+1)] * size_;
		else if ( iterBeg+3 == iterEnd ) return compressionIndexByChar[*iterBeg]
        									  + compressionIndexByChar[*(iterBeg+1)] * size_
        									  + compressionIndexByChar[*(iterBeg+2)] * Power<size_,2>::value;
		else if ( iterBeg+4 == iterEnd ) return compressionIndexByChar[*iterBeg]
											  + compressionIndexByChar[*(iterBeg+1)] * size_
											  + compressionIndexByChar[*(iterBeg+2)] * Power<size_,2>::value
											  + compressionIndexByChar[*(iterBeg+3)] * Power<size_,3>::value;
		else throw GeneAssemblerException("CompressedCharFourInOne: error in compress iter");
	}

	template < class Iterator >
	char compress( Iterator const& iterBeg,	Iterator const& iterEnd,
		typename boost::enable_if< boost::is_same<typename Iterator::value_type,CompressedCharTwoInOne> >::type* = 0 ) const
	{
		if ( iterBeg == iterEnd ) return 0;
		else if ( iterBeg+1 == iterEnd ) return iterBeg->template getCompressedIndex<0>()
				                              + iterBeg->template getCompressedIndex<1>() * size_;
		else if ( iterBeg+2 == iterEnd ) return iterBeg->template getCompressedIndex<0>()
                                              + iterBeg->template getCompressedIndex<1>() * size_
                                              + (iterBeg+1)->template getCompressedIndex<0>() * Power<size_,2>::value
                                              + (iterBeg+1)->template getCompressedIndex<1>() * Power<size_,3>::value;
		else throw GeneAssemblerException("CompressedCharFourInOne: error in compress iter");
	}

	std::string decompress() const
	{
		return std::string(1,getChar<0>()) + std::string(1,getChar<1>())
			+ std::string(1,getChar<2>()) + std::string(1,getChar<3>());
	}

	static const char compressionIndexByChar[256];
	static const char charByCompressionIndex[size_];

	char baseItem_;

};

inline std::ostream& operator << ( std::ostream& os, CompressedCharFourInOne const& c )
{
	return os << c.getString();
}

} // namespace GeneAssembler

namespace std {

template <>
struct hash< GeneAssembler::CompressedCharFourInOne >
{
	size_t operator () ( GeneAssembler::CompressedCharFourInOne const& c ) const
	{
		return hash<char>()(c.baseItem_);
	}
};

template <>
struct equal_to< GeneAssembler::CompressedCharFourInOne >
{
	bool operator () ( GeneAssembler::CompressedCharFourInOne const& c1, GeneAssembler::CompressedCharFourInOne const& c2 ) const
	{
		return equal_to<char>()(c1.baseItem_,c2.baseItem_);
	}
};

} // namespace std

#endif /* COMPRESSEDCHARFOURINONE_H_ */
