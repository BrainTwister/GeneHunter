#ifndef CDSENTRY_H_
#define CDSENTRY_H_

#include "ProteinLink.h"
#include <boost/serialization/access.hpp>
#include <iostream>
#include <string>

namespace GeneAssembler {

class CDSEntry
{
public:

	size_t geneID_ = 0;

	std::string referenceName_;

	std::string location_;

	PtrProteinLink ptrProteinLink_ = PtrProteinLink(new ProteinLink);

	void clearWithoutGeneID() {
		location_.clear();
		ptrProteinLink_ = PtrProteinLink(new ProteinLink);
	}

private:

	friend std::ostream& operator << ( std::ostream& os, CDSEntry const& entry );

    friend class boost::serialization::access;

    template < class Archive >
    void serialize( Archive & ar, const unsigned int )
    {
        ar & geneID_;
        ar & referenceName_;
        ar & location_;
        ar & ptrProteinLink_;
    }
};

inline std::ostream& operator << ( std::ostream& os, CDSEntry const& entry )
{
	return os << "geneID           = " << entry.geneID_ << std::endl
	          << "referenceName    = " << entry.referenceName_ << std::endl
	          << "location         = " << entry.location_ << std::endl
              << *entry.ptrProteinLink_ << std::endl;
}

} // namespace GeneAssembler

#endif /* CDSENTRY_H_ */
