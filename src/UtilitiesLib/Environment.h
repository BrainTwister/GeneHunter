#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_

#include "UtilitiesLib/Filesystem.h"

namespace GeneHunter {
namespace Environment {

filesystem::path getDatabaseFile();
filesystem::path getSharedDirectory();
filesystem::path getLocalDirectory();
filesystem::path getGeneHunterRootDirectory();
size_t getNbNodes();
size_t getNbCoresPerNode();
size_t getNodeMemoryGB();
std::string getMysqlHost();
std::string getMysqlUser();
std::string getMysqlPassword();
std::string getMysqlTaxDatabase();
std::string getMysqlMyDatabase();

} // namespace Environment
} // namespace GeneHunter

#endif /* ENVIRONMENT_H_ */
