#ifndef CDSITERATOR_H_
#define CDSITERATOR_H_

#include "GenomeLib/CDSEntry.h"
#include "UtilitiesLib/CreateDataClass.h"
#include "UtilitiesLib/Filesystem.h"
#include "UtilitiesLib/GeneHunterException.h"
#include <boost/algorithm/string/trim.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/tokenizer.hpp>
#include <istream>

namespace GeneHunter {

class CDSIterator
 : public boost::iterator_facade <
       CDSIterator,
          CDSEntry const,
          boost::forward_traversal_tag
   >
{
public:

    CREATE_DATA_CLASS( Settings,\
        (( bool, onlyImportKeyFeaturesWithPrecedingGeneSection, false ))\
        (( std::vector<std::string>, listOfFeatureKeysToImport, std::vector<std::string>{"CDS"} ))\
    )

    CDSIterator()
     : endFlag_(true)
    {}

    CDSIterator( boost::shared_ptr<std::istream> ptrInputStream, Settings const& settings )
     : ptrInputStream_(ptrInputStream), settings_(settings)
    {
        increment();
    }

private:

    friend class boost::iterator_core_access;

    CDSEntry const& dereference() const { return currentEntry_; }

    bool equal( CDSIterator const& other ) const {
        return endFlag_ == other.endFlag_;
    }

    void increment();

    void readGeneID();

    void readCDS();

    void removeQuotationMarks( std::string& str ) const;

    bool currentLineContainFeatureKey() const;

    enum CurrentReading { Gene, Location, LocusTag, ProteinID, Product, Unused };

    boost::shared_ptr<std::istream> ptrInputStream_;

    CDSEntry currentEntry_;

    std::string currentLine_;

    bool endFlag_ = false;

    static size_t indention_;

    Settings settings_;

};

} // namespace GeneHunter

#endif /* CDSITERATOR_H_ */
