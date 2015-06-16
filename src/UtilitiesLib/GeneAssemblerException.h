#ifndef GENEASSEMBLEREXCEPTION_H_
#define GENEASSEMBLEREXCEPTION_H_

#include <stdexcept>
#include <cassert>

struct GeneHunterException : public std::runtime_error {
    GeneHunterException( std::string const& msg ) : std::runtime_error(msg) {}
};

//struct GeneHunterException {
//    GeneHunterException( std::string const& msg ) { assert(false); }
//    std::string what() const { return "error"; }
//};

#endif /* GENEASSEMBLEREXCEPTION_H_ */
