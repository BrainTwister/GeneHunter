#ifndef ENVIRONMENTVARIABLES_H_
#define ENVIRONMENTVARIABLES_H_

#include <string>

namespace GeneHunter {

std::string getEnvVar( const std::string& key );

void setEnvVar( const std::string& key, const std::string& value, bool overwrite = true );

void unsetEnvVar( const std::string& key );

} // namespace GeneHunter

#endif /* ENVIRONMENTVARIABLES_H_ */
