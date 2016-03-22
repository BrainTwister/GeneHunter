#ifndef CHARTYPES_H_
#define CHARTYPES_H_

#include "CompressedCharTwoInOne.h"
#include "CompressedCharThreeInOne.h"
#include "CompressedCharFourInOne.h"

namespace GeneHunter {

using uchar = unsigned char;

template < class T >
struct GetNbBaseItemsInChar
{
    static const size_t value = T::CompressionFactor;
};

template < >
struct GetNbBaseItemsInChar<char>
{
    static const size_t value = 1;
};

template < class T >
inline bool containChar( T c1, char c2 )
{
    return c1.containChar(c2);
}

inline bool containChar( char c1, char c2 )
{
    return c1 == c2;
}

template < class T >
inline bool containOnlyPureBases( T const& c )
{
    return c.containOnlyPureBases();
}

inline bool containOnlyPureBases( char c )
{
    return c == 'A' or c == 'C' or c == 'G' or c == 'T';
}

/**
 * Return true if the first base match the second base.
 *
 * Special bases:
 *
 *   U   uridine (match T)
 *   K   G or T
 *   S   G or C
 *   R   G or A
 *   M   A or C
 *   W   A or T
 *   Y   T or C
 *   B   G, T, or C
 *   D   G, A, or T
 *   H   A, C, or T
 *   V   G, C, or A
 *   N   A, C, G, or T
 */
bool firstMatchSecond( char c1, char c2 );

} // namespace GeneHunter

#endif /* CHARTYPES_H_ */
