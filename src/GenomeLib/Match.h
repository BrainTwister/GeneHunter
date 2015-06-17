#ifndef MATCH_H_
#define MATCH_H_

#include "Genome.h"
#include "Helpers.h"
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <string>
#include <unordered_map>
#include <iostream>

namespace GeneHunter {

struct Match
{
	Match( size_t nuclGI = 0, size_t refStart = 0, size_t refEnd = 0,
		QualityType quality = 0.0, size_t nbTokenMatches = 0
	) : nuclGI_(nuclGI), refStart_(refStart), refEnd_(refEnd), quality_(quality), nbTokenMatches_(nbTokenMatches)
	{}

	bool operator == ( Match const& other ) const
    {
		return nuclGI_ == other.nuclGI_ and
			   refStart_ == other.refStart_ and
			   refEnd_ == other.refEnd_ and
			   quality_ == other.quality_ and
			   nbTokenMatches_ == other.nbTokenMatches_;
    }

	bool operator != ( Match const& other ) const
    {
		return ! ( *this == other );
    }

	size_t nuclGI_;
	size_t refStart_;
	size_t refEnd_;
	QualityType quality_;
	size_t nbTokenMatches_;

private:

    friend class boost::serialization::access;

    template < class Archive >
    void serialize( Archive & ar, const unsigned int version )
    {
        ar & BOOST_SERIALIZATION_NVP(nuclGI_);
        ar & BOOST_SERIALIZATION_NVP(refStart_);
        ar & BOOST_SERIALIZATION_NVP(refEnd_);
        ar & BOOST_SERIALIZATION_NVP(quality_);
        ar & BOOST_SERIALIZATION_NVP(nbTokenMatches_);
    }
};

using Matches = std::unordered_map<std::string,Match>;

inline std::ostream& operator << ( std::ostream& os, Match const& match )
{
	return os << "nuclGI         = " << match.nuclGI_ << std::endl
			  << "refStart       = " << match.refStart_ << std::endl
			  << "refEnd         = " << match.refEnd_ << std::endl
			  << "quality        = " << match.quality_ << std::endl
	          << "nbTokenMatches = " << match.nbTokenMatches_ << std::endl;
}

inline std::ostream& operator << ( std::ostream& os, Matches const& matches )
{
	for ( auto const& match : matches ) os << match << std::endl;
	return os;
}

} // namespace GeneHunter

#endif /* MATCH_H_ */
