#ifndef ORGANISMSREPORT_H_
#define ORGANISMSREPORT_H_

#include "GenomeLib/CDSDatabase.h"
#include "GenomeLib/Match.h"
#include "GenomeLib/Organism.h"
#include "GenomeLib/OrganismsDataset.h"
#include "GenomeLib/Taxonomy.h"
#include "UtilitiesLib/CreateDataClass.h"
#include <boost/filesystem/path.hpp>
#include <chrono>
#include <map>
#include <string>
#include <vector>

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

} // namespace GeneHunter

#endif /* ORGANISMSREPORT_H_ */
