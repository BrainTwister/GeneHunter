#ifndef BOOSTSERIALIZEARRAY_H_
#define BOOSTSERIALIZEARRAY_H_

#include <boost/config.hpp>
#include <boost/serialization/array.hpp>
#include <array>

#if ((BOOST_VERSION / 100) % 1000) < 58

namespace boost {
namespace serialization {

template < class Archive, class T, size_t N >
inline void serialize( Archive & ar, std::array<T,N>& a, const unsigned int version ) {
	ar & boost::serialization::make_array(a.data(), N);
}

} // serialization
} // boost

#endif /* BOOST_VERSION */
#endif /* BOOSTSERIALIZEARRAY_H_ */
