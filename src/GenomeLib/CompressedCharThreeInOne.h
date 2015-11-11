#ifndef COMPRESSEDCHARTHREEINONE_H_
#define COMPRESSEDCHARTHREEINONE_H_

#include "BoostLib/boost/serialization/array.h"
#include "UtilitiesLib/GeneHunterException.h"
#include <array>
#include <functional>
#include <string>

namespace GeneHunter {

struct CompressedCharThreeInOne
{
    CompressedCharThreeInOne( char base1, char base2 )
     : baseItem_( compress(base1) + compress(base2) * size_ )
    {}

    char compress( char base ) const
    {
        switch (base) {
            case 'A' : return 0;
            case 'C' : return 1;
            case 'G' : return 2;
            case 'T' : return 3;
            case 'N' : return 4;
            case 'X' : return 5;
            default : GeneHunterException("CompressedCharThreeInOne: stringToken contain unkown letter.");
        }
    }

    char decompress( char item ) const
    {
        switch (item) {
            case 0 : return 'A';
            case 1 : return 'C';
            case 2 : return 'G';
            case 3 : return 'T';
            case 4 : return 'N';
            case 5 : return 'X';
            default : GeneHunterException("CompressedCharThreeInOne: stringToken contain unkown item.");
        }
    }

    std::string getString() const {
        char base1 = decompress(baseItem_%size_);
        char base2 = decompress(baseItem_/size_);
        return std::string(&base1) + std::string(&base2);
    }

    char getChar() const
    {
        return baseItem_;
    }

private:

    char baseItem_;

    static const char size_ = 16;

};

} // namespace GeneHunter

#endif /* COMPRESSEDCHARTHREEINONE_H_ */
