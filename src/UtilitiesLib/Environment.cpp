#include "UtilitiesLib/Environment.h"
#include "UtilitiesLib/EnvironmentVariables.h"
#include "UtilitiesLib/Filesystem.h"
#include "UtilitiesLib/GeneHunterException.h"
#include <boost/lexical_cast.hpp>

namespace GeneHunter {
namespace Environment {

filesystem::path getDatabaseFile()
{
    std::string dbFileString = getEnvVar("NCBI_NT_PATH");
    if (dbFileString.empty()) throw GeneHunterException("NCBI_NT_PATH not defined");

    filesystem::path dbFile = filesystem::path(dbFileString) / "nt";
    if (!exists(dbFile)) throw GeneHunterException(dbFile.string() + " not exist");

    return dbFile;
}

filesystem::path getSharedDirectory()
{
    std::string scratchShared = getEnvVar("SCRATCH_SHARED");
    if (scratchShared.empty()) throw GeneHunterException("SCRATCH_SHARED not defined");

    filesystem::path scratchSharedPath = filesystem::path(scratchShared);
    if (!exists(scratchSharedPath)) throw GeneHunterException(scratchSharedPath.string() + " not exist");

    return scratchSharedPath;
}

filesystem::path getLocalDirectory()
{
    std::string localScratch = getEnvVar("SCRATCH_LOCAL");
    if (localScratch.empty()) throw GeneHunterException("SCRATCH_LOCAL not defined");

    filesystem::path localScratchPath = filesystem::path(localScratch);
    if (!exists(localScratchPath)) throw GeneHunterException(localScratchPath.string() + " not exist");

    return localScratchPath;
}

filesystem::path getGeneHunterRootDirectory()
{
    std::string geneHunterRoot = getEnvVar("GENEHUNTER_ROOT");
    if (geneHunterRoot.empty()) throw GeneHunterException("GENEHUNTER_ROOT not defined");

    filesystem::path geneHunterRootPath = filesystem::path(geneHunterRoot);
    if (!exists(geneHunterRootPath)) throw GeneHunterException(geneHunterRootPath.string() + " not exist");

    return geneHunterRootPath;
}

size_t getNbNodes() { return boost::lexical_cast<size_t>(getEnvVar("NODE_NUM")); }
size_t getNbCoresPerNode() { return boost::lexical_cast<size_t>(getEnvVar("NODE_CPUS")); }
size_t getNodeMemoryGB() { return boost::lexical_cast<size_t>(getEnvVar("NODE_MEM")); }

std::string getMysqlHost() { return getEnvVar("MYSQL_HOST"); }
std::string getMysqlUser() { return getEnvVar("MYSQL_USER"); }
std::string getMysqlPassword() { return getEnvVar("MYSQL_PASS"); }
std::string getMysqlTaxDatabase() { return getEnvVar("MYSQL_TAX_DB"); }
std::string getMysqlMyDatabase() { return getEnvVar("MYSQL_MY_DB"); }

} // namespace Environment
} // namespace GeneHunter
