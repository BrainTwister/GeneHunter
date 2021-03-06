#ifndef GETCOMPRESSEDINDEX_H_
#define GETCOMPRESSEDINDEX_H_

#include "UtilitiesLib/Power.h"

namespace GeneHunter {

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

} // namespace GeneHunter

#endif /* GETCOMPRESSEDINDEX_H_ */
