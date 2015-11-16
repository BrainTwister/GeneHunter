#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#ifdef GENEHUNTER_USE_BOOST_FILESYSTEM
    #include "BoostLib/boost/serialization/path.h"
    #include <boost/filesystem.hpp>
    namespace filesystem = boost::filesystem;
#else
    #include <experimental/filesystem>
    namespace filesystem = std::filesystem;
#endif

#endif /* FILESYSTEM_H_ */
