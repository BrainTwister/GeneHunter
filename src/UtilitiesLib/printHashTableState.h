#ifndef PRINTHASHTABLESTATE_H_
#define PRINTHASHTABLESTATE_H_

#include <iomanip>
#include <iostream>

namespace GeneHunter {

template < class T >
void printHashTableState( const T& cont, std::ostream& out )
{
    out << "size:                 " << cont.size() << std::endl;
    out << "buckets:              " << cont.bucket_count() << std::endl;
    out << "load factor:          " << cont.load_factor() << std::endl;
    out << "max load factor       " << cont.max_load_factor() << std::endl;

    size_t smallest_bucket = std::numeric_limits<size_t>::max();
    size_t largest_bucket = 0;
    size_t num_empty_bucket = 0;

    for ( auto idx(0); idx != cont.bucket_count(); ++idx )
    {
        if ( cont.bucket_size(idx) < smallest_bucket ) smallest_bucket = cont.bucket_size(idx);
        if ( cont.bucket_size(idx) > largest_bucket ) largest_bucket = cont.bucket_size(idx);
        if ( cont.bucket_size(idx) == 0 ) ++num_empty_bucket;
    }

    out << "smallest bucket:      " << smallest_bucket << std::endl;
    out << "largest bucket:       " << largest_bucket << std::endl;
    out << "number empty buckets: " << num_empty_bucket << std::endl;
    out << "average bucket:       " << static_cast<double>(cont.size()) / cont.bucket_count() << std::endl;
}

template < class T >
void printHashTableBucketEntries( const T& cont, std::ostream& out )
{
    for ( auto idx(0); idx != cont.bucket_count(); ++idx )
    {
        out << "b[" << std::setw(2) << idx << "]: ";
        for ( auto pos(cont.begin(idx)); pos != cont.end(idx); ++pos ) {
            out << pos->first << " ";
        }
        out << std::endl;
    }
}

} // namespace GeneHunter

#endif /* PRINTHASHTABLESTATE_H_ */
