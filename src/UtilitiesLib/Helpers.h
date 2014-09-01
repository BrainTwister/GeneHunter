#ifndef HELPERS_H_
#define HELPERS_H_

#include <iostream>
#include <unordered_map>
#include <map>
#include <tuple>

namespace GeneAssembler {

template < class Key, class T >
inline std::ostream& operator << ( std::ostream& os, std::map<Key,T> const& m )
{
	os << "map[";
	for ( auto const& e : m ) os << e;
	os << "]";
	return os;
}

template < class Key, class T >
inline std::ostream& operator << ( std::ostream& os, std::unordered_map<Key,T> const& m )
{
	os << "unordered_map[";
	for ( auto const& e : m ) os << e;
	os << "]";
	return os;
}

template < class T >
inline std::ostream& operator << ( std::ostream& os, std::vector<T> const& v )
{
	os << "vector[";
	for ( auto const& e : v ) os << e;
	os << "]";
	return os;
}

template < class T1, class T2 >
inline std::ostream& operator << ( std::ostream& os, std::pair<T1,T2> const& p )
{
	return os << "pair[" << p.first << "," << p.second << "]";
}

template < class T >
inline std::ostream& operator << ( std::ostream& os, std::shared_ptr<T> const& p )
{
	return os << *p;
}

template < class ... T >
inline std::ostream& operator << ( std::ostream& os, std::tuple< T ... > const& t )
{
	return os << "tuple[" << std::get<0>(t) << "]";
}

} // namespace GeneAssembler

#endif /* HELPERS_H_ */
