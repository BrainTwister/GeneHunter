#include "StringUtilities.h"
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <iostream>

std::set<size_t> readListing( const std::string& listingString )
{
    std::set<size_t> listing;
    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
    tokenizer tok(listingString,boost::char_separator<char>(","));
    for( tokenizer::iterator beg(tok.begin()), end(tok.end()); beg != end; ++beg ) {
        tokenizer tok2(*beg,boost::char_separator<char>("-"));
        tokenizer::iterator iterToken2(tok2.begin());

        size_t first = boost::lexical_cast<size_t>(*iterToken2);
        ++iterToken2;
        if ( iterToken2 == tok2.end()) {
            listing.insert(first);
            continue;
        }

        size_t second = boost::lexical_cast<size_t>(*iterToken2);
        ++iterToken2;
        if ( iterToken2 == tok2.end()) {
            for ( size_t i(first); i <= second; ++i ) listing.insert(i);
            continue;
        }

        throw std::runtime_error("readListing: ");
    }
    return listing;
}

std::string makeFrame( std::string const& content, char letter )
{
    return std::string(content.size() + 6,letter) + '\n' + letter + "  "
        + content + "  " + letter + '\n' + std::string(content.size() + 6,letter);
}

std::string underline( std::string const& content )
{
    return content + '\n' + std::string(content.size(),'-');
}

std::string removePrependingLowerLetters( std::string const& str )
{
    auto iterFirstUpperChar = str.begin();
    for ( ; iterFirstUpperChar != str.end(); ++iterFirstUpperChar ) if ( std::isupper(*iterFirstUpperChar) ) break;
    if (iterFirstUpperChar == str.end()) return str;
    return str.substr(distance(str.begin(),iterFirstUpperChar));
}

std::string removePrependingLowerLettersByReference( std::string const& str, std::string const& ref )
{
    auto iterFirstUpperChar = ref.begin();
    for ( ; iterFirstUpperChar != ref.end(); ++iterFirstUpperChar ) if ( std::isupper(*iterFirstUpperChar) ) break;
    if (iterFirstUpperChar == ref.end()) return str;
    return str.substr(distance(ref.begin(),iterFirstUpperChar));
}
