#ifndef CDSDATABASEMEMORYBASED_H_
#define CDSDATABASEMEMORYBASED_H_

#include "CreateDataClass.h"
#include "CDSEntry.h"
#include "Gene.h"
#include "Match.h"
#include "ProteinLink.h"
#include <boost/filesystem.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/map.hpp>
#include "boost/serialization/path.h"
#include <map>
#include <fstream>
#include <string>

namespace GeneHunter {

/**
 * CDSDatabase holding data in memory.
 */
class CDSDatabaseMemoryBased
{
	typedef std::map<Location, PtrProteinLink> LocationLink;
	typedef std::map<size_t, LocationLink> DatabaseType;

public:

	bool getGene( Match const& match ) const
	{
		DatabaseType::const_iterator iterDatabaseEntry = database_.find(match.geneID_);
		if ( iterDatabaseEntry == database_.end() ) return false;
	}

	void importGene( CDSEntry const& entry )
	{
		DatabaseType::iterator iterDatabaseEntry = database_.find(entry.geneID_);
		if ( iterDatabaseEntry == database_.end() )
			iterDatabaseEntry = database_.insert(database_.begin(), std::make_pair(entry.geneID_,entry.location_));
		iterDatabaseEntry->second.insert(std::make_pair(Location(entry.location_),entry.ptrProteinLink_));
	}

private:

	friend class boost::serialization::access;

	template < class Archive >
	void serialize( Archive & ar, const unsigned int )
	{
		ar & database_;
	}

	DatabaseType database_;

};

} // namespace GeneHunter

#endif /* CDSDATABASEMEMORYBASED_H_ */
