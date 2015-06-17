#ifndef CDSDATABASE_H_
#define CDSDATABASE_H_

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
#include <mysql.h>
#include <fstream>
#include <string>

namespace GeneHunter {

/**
 * CDS "coding sequence" refers to a location of the nucleotides, which could be translated to
 * a protein. The CDSDatabase is responsible to find the correct connection. In many cases a \ref Location
 * is divided in sub-ranges. A enclosingLocation ignores these sub-ranges and only define the lower
 * bound of the first range and the upper bound of the last range. Using the enclosingLocation
 * enables a faster search, which can be refine afterwards using the real range set.
 */
class CDSDatabase
{
public:

    CREATE_DATA_CLASS( Settings,\
        (( std::string, proteinLinkTableName, "ProteinLink" ))\
        (( uint8_t, verbosity, 0 ))\
        (( bool, checkSubdividedLocation, true ))\
    )

    CDSDatabase( Settings const& settings );

    ~CDSDatabase();

    std::vector<Gene> getGene( size_t geneID, size_t locStart, size_t locEnd ) const;

    void importGene( CDSEntry const& entry );

    size_t nbGeneEntries() const;

    size_t nbProteinLinks() const;

    void createIndex() const;

private:

    /// Produce a valid sql string.
    std::string strip( std::string const& s ) const;

    Settings settings_;

    MYSQL *myConnection_;

    static const uint16_t stripBufferLength_ = 10000;

    /// Buffer for striping to reduces heap allocations.
    mutable char stripBuffer_[stripBufferLength_];

};

} // namespace GeneHunter

#endif /* CDSDATABASE_H_ */
