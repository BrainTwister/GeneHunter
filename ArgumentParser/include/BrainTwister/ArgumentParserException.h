#ifndef ARGUMENTPARSEREXCEPTION_H_
#define ARGUMENTPARSEREXCEPTION_H_

#include <stdexcept>

struct ArgumentParserException : public std::runtime_error {
    ArgumentParserException(const std::string &msg) : std::runtime_error(msg) {}
};

#endif /* ARGUMENTPARSEREXCEPTION_H_ */
