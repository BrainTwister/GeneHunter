#ifndef FASTQ_H_
#define FASTQ_H_

#include "GenomeLib/Genome.h"
#include "GenomeLib/Sequence.h"
#include "UtilitiesLib/GeneHunterException.h"
#include <boost/serialization/shared_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <iostream>
#include <string>

namespace GeneHunter {

class FASTQ
{
public:

    FASTQ() {}

    FASTQ( std::string const& name, std::string const& sequence, std::string const& quality )
     : name_(name), sequence_(sequence), quality_(quality)
    {}

    std::string const& getName() const { return name_; }
    Sequence<ReadSeqCharType> const& getSequence() const { return sequence_; }
    Sequence<ReadQualCharType> const& getQuality() const { return quality_; }

private:

    friend class boost::serialization::access;

    template < class Archive >
    void serialize( Archive & ar, const unsigned int )
    {
        ar & BOOST_SERIALIZATION_NVP(name_);
        ar & BOOST_SERIALIZATION_NVP(sequence_);
        ar & BOOST_SERIALIZATION_NVP(quality_);
    }

    std::string name_;

    Sequence<ReadSeqCharType> sequence_;

    Sequence<ReadQualCharType> quality_;

};

using PtrFASTQ = boost::shared_ptr<FASTQ>;

inline std::ostream& operator << ( std::ostream& os, FASTQ const& fastq )
{
    return os << "name     = " << fastq.getName() << std::endl
              << "sequence = " << fastq.getSequence() << std::endl
              << "quality  = " << fastq.getQuality() << std::endl;
}

} // namespace GeneHunter

#endif /* FASTQ_H_ */
