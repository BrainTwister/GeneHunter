#ifndef TAXONOMYWRITER_H_
#define TAXONOMYWRITER_H_

#include <boost/filesystem/path.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/map.hpp>
#include "BoostLib/boost/serialization/path.h"
#include <mysql.h>
#include <fstream>
#include <string>

namespace GeneHunter {

class TaxonomyWriter
{
public:

    TaxonomyWriter( boost::filesystem::path const& gi_taxid_nucl_file, boost::filesystem::path const& names_file,
        boost::filesystem::path const& nodes_file, std::string const& database );

    ~TaxonomyWriter();

    void importGITaxIDNuc( boost::filesystem::path const& filename );

    void importNCBINames( boost::filesystem::path const& filename );

    void importNCBINodes( boost::filesystem::path const& filename );

    void createIndices() const;

private:

    /// Produce a valid sql string.
    std::string strip( std::string const& s ) const;

    MYSQL *myConnection_;

    static const uint16_t stripBufferLength_ = 10000;

    /// Buffer for striping to reduces heap allocations.
    mutable char stripBuffer_[stripBufferLength_];

    static const std::string giTaxIDNucTableName_;

    static const std::string ncbiNodesTableName_;

    static const std::string ncbiNamesTableName_;

};

} // namespace GeneHunter

#endif /* TAXONOMYWRITER_H_ */
