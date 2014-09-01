#ifndef GETCOMPRESSEDINDEX_H_
#define GETCOMPRESSEDINDEX_H_

#include "Power.h"

namespace GeneAssembler {

template < size_t size, size_t factorMinus1, size_t pos >
struct GetCompressedIndex
{
	char operator () ( char c ) {
	    return c % Power<size,pos+1>::value / Power<size,pos>::value;
	}
};

template < size_t size, size_t pos >
struct GetCompressedIndex<size,pos,pos>
{
	char operator () ( char c ) {
	    return c / Power<size,pos>::value;
	}
};

template < size_t size, size_t factorMinus1 >
struct GetCompressedIndex<size,factorMinus1,0>
{
	char operator () ( char c ) {
	    return c % size;
	}
};

} // namespace GeneAssembler

#endif /* GETCOMPRESSEDINDEX_H_ */
