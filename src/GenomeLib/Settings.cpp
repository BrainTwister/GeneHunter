#include "GenomeLib/Settings.h"
#include "UtilitiesLib/GeneHunterException.h"
#include <string>

namespace GeneHunter {

std::ostream& operator << ( std::ostream& os, SearchAlgorithm s )
{
    switch (s) {
        case FirstTokensAsSeed :
            return os << "FirstTokensAsSeed";
        case TakeMatchWithMostSeeds :
            return os << "TakeMatchWithMostSeeds";
        default:
            throw GeneHunterException("FullMatchManager: unkown searchAlgorithm");
    }
}

std::istream& operator >> ( std::istream& is, SearchAlgorithm& s )
{
    std::string str;
    is >> str;
    if ( str == "FirstTokensAsSeed" )
        s = FirstTokensAsSeed;
    else if ( str == "TakeMatchWithMostSeeds" )
        s = TakeMatchWithMostSeeds;
    else
           throw GeneHunterException("FullMatchManager: unkown searchAlgorithm");
    return is;
}

} // namespace GeneHunter
