#ifndef QUALITY_H_
#define QUALITY_H_

#include <algorithm>
#include <array>

namespace GeneHunter {

template < char ... >
struct seq {};

template < uint8_t N, char ...S >
struct gens : gens< N-1, N-1, S... > {};

template < char ...S >
struct gens< 0, S... >
{
    typedef seq< S... > type;
};

constexpr double quality( char c, uint8_t offset ) {
    return c > offset ? pow(10.0,-static_cast<double>(c-offset)/10) : 1.0;
}

/**
 * PHRED quality score is stored as a single ASCII letter. Since ASCII 0-32 are not printable
 * a offset of 33 will be used.
 *
 *   Q_PHRED = -10 * log10(P_e);
 *
 * P_e is the probability that the base is the correct one. Using PHRED qualities from 0 to 93 (ASCII 33-126)
 * the error probabilities range from 1 (wrong base) to 1e-9.3 (extremely accurate read).
 */
struct FastqSanger
{
    static constexpr uint8_t size = 127;
    static constexpr uint8_t offset = 33;

    typedef typename gens<size>::type list;

    template < char ...S >
    static constexpr std::array<double,size> make_arr(seq<S...>) {
        return std::array<double,size>{{ quality(S,offset) ... }};
    }

    static const std::array<double,size> data;

};

} // namespace GeneHunter

#endif /* QUALITY_H_ */
