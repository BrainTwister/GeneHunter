#ifndef GENEASSEMBLEREXCEPTION_H_
#define GENEASSEMBLEREXCEPTION_H_

#include <stdexcept>
#include <cassert>

struct GeneAssemblerException : public std::runtime_error {
    GeneAssemblerException( std::string const& msg ) : std::runtime_error(msg) {}
};

//struct GeneAssemblerException {
//    GeneAssemblerException( std::string const& msg ) { assert(false); }
//    std::string what() const { return "error"; }
//};

#endif /* GENEASSEMBLEREXCEPTION_H_ */
