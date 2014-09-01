#ifndef HASHFUNCTIONS_H_
#define HASHFUNCTIONS_H_

namespace std {

template < class T, class U >
struct hash< pair<T,U> >
{
	size_t operator()( pair<T,U> x ) const {
		return hash<T>()(x.first) ^ hash<U>()(x.second);
	}
};

/**
 * The comparison between unordered_map objects is not affected by the arbitrary order in which they store
 * their elements. Two unordered_maps are equal if they have the same number of elements and the elements
 * in one container are a permutation of the elements in the other container. Otherwise, they are unequal.
 */
//template < class T1, class T2 >
//bool operator == ( Matches<T1> const& m1, Matches<T2> const& m2 )
//{
//	if ( m1.size() != m2.size() ) return false;
//	for ( auto const& e1 : m1 )
//	{
//		auto i2 = m2.find(e1.first);
//		if ( i2 == m2.end() ) return false;
//		if ( e1.second != i2->second ) return false;
//	}
//	return true;
//}
//
//template < class T1, class T2 >
//bool operator != ( Matches<T1> const& m1, Matches<T2> const& m2 )
//{
//	return !(m1 == m2);
//}

} // namespace std

#endif /* HASHFUNCTIONS_H_ */
