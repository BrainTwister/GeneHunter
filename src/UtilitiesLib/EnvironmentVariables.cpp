#include "EnvironmentVariables.h"
#include "GeneAssemblerException.h"
#include <cstdlib>

namespace GeneAssembler {

std::string getEnvVar( const std::string& key )
{
    const char* value = getenv(key.c_str());
    if ( value == NULL ) throw GeneAssemblerException("getEnvVar: env " + key + " is not set.");
    return std::string(value);
}

void setEnvVar( const std::string& key, const std::string& value, bool overwrite )
{
    if ( setenv(key.c_str(), value.c_str(), overwrite) != 0 )
        throw GeneAssemblerException("setEnvVar: cannot set env " + key + "." );
}

void unsetEnvVar( const std::string& key )
{
    if ( unsetenv(key.c_str()) != 0 )
        throw GeneAssemblerException("unsetEnvVar: env " + key + " is not set.");
}

} // namespace GeneAssembler
