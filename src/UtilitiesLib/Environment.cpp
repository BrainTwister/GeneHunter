#include "Environment.h"
#include "EnvironmentVariables.h"
#include "GeneAssemblerException.h"
#include <boost/lexical_cast.hpp>

namespace GeneAssembler {
namespace Environment {

boost::filesystem::path getDatabaseFile()
{
	std::string dbFileString = getEnvVar("NCBI_NT_PATH");
	if (dbFileString.empty()) throw GeneAssemblerException("NCBI_NT_PATH not defined");

	boost::filesystem::path dbFile = boost::filesystem::path(dbFileString) / "nt";
	if (!exists(dbFile)) throw GeneAssemblerException(dbFile.string() + " not exist");

	return dbFile;
}

boost::filesystem::path getSharedDirectory()
{
	std::string scratchShared = getEnvVar("SCRATCH_SHARED");
	if (scratchShared.empty()) throw GeneAssemblerException("SCRATCH_SHARED not defined");

	boost::filesystem::path scratchSharedPath = boost::filesystem::path(scratchShared);
	if (!exists(scratchSharedPath)) throw GeneAssemblerException(scratchSharedPath.string() + " not exist");

	return scratchSharedPath;
}

boost::filesystem::path getLocalDirectory()
{
	std::string localScratch = getEnvVar("SCRATCH_LOCAL");
	if (localScratch.empty()) throw GeneAssemblerException("SCRATCH_LOCAL not defined");

	boost::filesystem::path localScratchPath = boost::filesystem::path(localScratch);
	if (!exists(localScratchPath)) throw GeneAssemblerException(localScratchPath.string() + " not exist");

	return localScratchPath;
}

boost::filesystem::path getGeneAssemblerRootDirectory()
{
	std::string geneAssemblerRoot = getEnvVar("GENEASSEMBLER_ROOT");
	if (geneAssemblerRoot.empty()) throw GeneAssemblerException("GENEASSEMBLER_ROOT not defined");

	boost::filesystem::path geneAssemblerRootPath = boost::filesystem::path(geneAssemblerRoot);
	if (!exists(geneAssemblerRootPath)) throw GeneAssemblerException(geneAssemblerRootPath.string() + " not exist");

	return geneAssemblerRootPath;
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
} // namespace GeneAssembler
