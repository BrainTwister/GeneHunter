#ifndef ORGANISMSREPORT_H_
#define ORGANISMSREPORT_H_

#include "CreateDataClass.h"
#include "CDSDatabase.h"
#include "Match.h"
#include "Organism.h"
#include "OrganismsDataset.h"
#include "Taxonomy.h"
#include <boost/filesystem/path.hpp>
#include <chrono>
#include <map>
#include <string>
#include <vector>

namespace GeneAssembler {

class OrganismsReport
{
public:

	CREATE_DATA_CLASS( Settings,\
	    (( CDSDatabase::Settings, cdsDatabaseSettings, CDSDatabase::Settings() ))\
	    (( Taxonomy::Settings, taxonomySettings, Taxonomy::Settings() ))\
		(( bool, removeUnwantedOrganisms, false ))\
		(( size_t, minReadsForOrganism, 3 ))\
		(( size_t, matchNotificationNumber, 1000 ))\
		(( uint8_t, verbosity, 0 ))\
	)

	OrganismsReport( boost::filesystem::path const& reportFile, boost::filesystem::path const& datasetFile,
		Settings const& settings, size_t nbReads );

	~OrganismsReport();

	void addMatches( Matches const& matches );

private:

	typedef boost::shared_ptr<Organism> PtrOrganism;
	typedef std::vector<PtrOrganism> Organisms;
	typedef std::unordered_map<size_t,PtrOrganism> OrganismLookupTable;

    OrganismsDataset dataset;

    Organisms organisms;

    OrganismLookupTable organismLookupTable;

    size_t nbReads;

    boost::filesystem::path reportFile;

    Taxonomy taxonomy;

    CDSDatabase cdsDatabase;

    Settings settings;

    std::set<NuclGIType> geneIDsNotFountInTaxonomy;

};

} // namespace GeneAssembler

#endif /* ORGANISMSREPORT_H_ */
