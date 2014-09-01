#ifndef STRINGUTILITIES_h_
#define STRINGUTILITIES_h_

#include <fstream>
#include <set>
#include <string>

std::set<size_t> readListing( const std::string& listing );

std::string makeFrame( std::string const& content, char letter);

std::string underline( std::string const& content );

std::string removePrependingLowerLetters( std::string const& str );

std::string removePrependingLowerLettersByReference( std::string const& str, std::string const& ref );

#endif /* STRINGUTILITIES_h_ */
