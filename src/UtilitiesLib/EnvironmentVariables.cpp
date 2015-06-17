#include "EnvironmentVariables.h"
#include "GeneHunterException.h"
#include <cstdlib>

namespace GeneHunter {

std::string getEnvVar( const std::string& key )
{
    const char* value = getenv(key.c_str());
    if ( value == NULL ) throw GeneHunterException("getEnvVar: env " + key + " is not set.");
    return std::string(value);
}

void setEnvVar( const std::string& key, const std::string& value, bool overwrite )
{
    if ( setenv(key.c_str(), value.c_str(), overwrite) != 0 )
        throw GeneHunterException("setEnvVar: cannot set env " + key + "." );
}

void unsetEnvVar( const std::string& key )
{
    if ( unsetenv(key.c_str()) != 0 )
        throw GeneHunterException("unsetEnvVar: env " + key + " is not set.");
}

} // namespace GeneHunter
