#ifndef PROTEINLINK_H_
#define PROTEINLINK_H_

#include <string>
#include <boost/lexical_cast.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/shared_ptr.hpp>

namespace GeneHunter {

struct ProteinLink
{
	std::string gene_;
	std::string locusTag_;
	std::string proteinID_;
	std::string product_;

private:

	friend std::ostream& operator << ( std::ostream& os, ProteinLink const& proteinLink );

    friend class boost::serialization::access;

    template < class Archive >
    void serialize( Archive & ar, const unsigned int )
    {
        ar & gene_;
        ar & locusTag_;
        ar & proteinID_;
        ar & product_;
    }
};

typedef boost::shared_ptr<ProteinLink> PtrProteinLink;

inline std::ostream& operator << ( std::ostream& os, ProteinLink const& proteinLink )
{
	return os << "gene             = " << boost::lexical_cast<std::string>(proteinLink.gene_) << std::endl
	          << "locusTag         = " << proteinLink.locusTag_ << std::endl
	          << "proteinID        = " << proteinLink.proteinID_ << std::endl
              << "product          = " << proteinLink.product_ << std::endl;
}

} // namespace GeneHunter

#endif /* PROTEINLINK_H_ */
