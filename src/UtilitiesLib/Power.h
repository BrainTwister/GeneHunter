#ifndef POWER_H_
#define POWER_H_

#include <boost/mpl/arithmetic.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/greater_equal.hpp>
#include <boost/mpl/long.hpp>
#include <boost/mpl/range_c.hpp>

template < long A, long B >
struct Power {

    BOOST_MPL_ASSERT(( boost::mpl::greater_equal< boost::mpl::long_<B>, boost::mpl::long_<0> > ));

    typedef typename boost::mpl::range_c< long, 0, B > sequence;
    typedef typename boost::mpl::fold<
        sequence,
        boost::mpl::long_< 1 >,
        boost::mpl::times< boost::mpl::_1, boost::mpl::long_< A > >
    >::type type;
    static const long value = type::value;
};

#endif /* POWER_H_ */
