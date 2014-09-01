#ifndef BOOSTSERIALIZEPATH_H_
#define BOOSTSERIALIZEPATH_H_

#include <boost/serialization/nvp.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/serialization/level.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/split_free.hpp>
#include <iostream>

namespace boost {
namespace serialization {

template<class Archive>
void save(Archive& ar, const boost::filesystem::path& p, const unsigned int version)
{
    std::string s(p.string());
    ar & boost::serialization::make_nvp("string", s);
}

template<class Archive>
void load(Archive& ar, boost::filesystem::path& p, const unsigned int version)
{
    std::string s;
    ar & boost::serialization::make_nvp("string", s);
    p = s;
}

template<class Archive>
inline void serialize(Archive & ar, boost::filesystem::path& p, const unsigned int version) {
    split_free(ar, p, version);
}

} // serialization
} // boost

#endif /* BOOSTSERIALIZEPATH_H_ */
