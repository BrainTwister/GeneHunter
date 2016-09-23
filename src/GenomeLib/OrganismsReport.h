#ifndef ORGANISMSREPORT_H_
#define ORGANISMSREPORT_H_

#include <boost/filesystem/path.hpp>
#include <stddef.h>
#include <set>
#include <vector>

#include "CDSDatabase.h"
#include "Genome.h"
#include "Match.h"
#include "Organism.h"
#include "OrganismsDataset.h"
#include "Taxonomy.h"
#include "UtilitiesLib/CreateDataClass.h"
#include "UtilitiesLib/Filesystem.h"

namespace GeneHunter {

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

    OrganismsReport(filesystem::path const& reportFile, filesystem::path const& datasetFile,
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

    filesystem::path reportFile;

    Taxonomy taxonomy;

    size_t nbReads;

    CDSDatabase cdsDatabase;

    Settings settings;

    std::set<NuclGIType> geneIDsNotFountInTaxonomy;

};

} // namespace GeneHunter

#endif /* ORGANISMSREPORT_H_ */
