#include "CharTypes.h"

namespace GeneHunter {

bool firstMatchSecond( char c1, char c2 )
{
    if ( c1 == c2 ) return true;
    if ( c1 == 'U' and c2 == 'T' ) return true;
    if ( c1 == 'T' and c2 == 'U' ) return true;
    if ( c1 == 'K' and ( c2 == 'G' or c2 == 'T' )) return true;
    if ( c1 == 'S' and ( c2 == 'G' or c2 == 'C' )) return true;
    if ( c1 == 'R' and ( c2 == 'G' or c2 == 'A' )) return true;
    if ( c1 == 'M' and ( c2 == 'A' or c2 == 'C' )) return true;
    if ( c1 == 'W' and ( c2 == 'A' or c2 == 'T' )) return true;
    if ( c1 == 'Y' and ( c2 == 'T' or c2 == 'C' )) return true;
    return false;
}

} // namespace GeneHunter
