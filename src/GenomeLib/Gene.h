#ifndef GENE_H_
#define GENE_H_

#include "ProteinLink.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <string>

namespace GeneHunter {

struct Gene
{
    Gene(
        std::string const& gene = "",
        std::string const& locusTag = "",
        std::string const& proteinID = "",
        std::string const& product = "",
        std::string const& refName = ""
    ) :
        gene(gene),
        locusTag(locusTag),
        proteinID(proteinID),
        product(product),
        refName(refName)
    {}

    std::string gene;
    std::string locusTag;
    std::string proteinID;
    std::string product;
    std::string refName;

    bool operator == ( Gene const& other ) const
    {
        return gene == other.gene and
               locusTag == other.locusTag and
               proteinID == other.proteinID and
               product == other.product and
               refName == other.refName;
    }

    bool operator < ( Gene const& other ) const
    {
        if (gene < other.gene) return true;
        if (gene == other.gene) {
            if (locusTag < other.locusTag) return true;
            if (locusTag == other.locusTag) {
                if (proteinID < other.proteinID) return true;
                if (proteinID == other.proteinID) {
                    if (product < other.product) return true;
                    if (product == other.product) {
                        if (refName < other.refName) return true;
                    }
                }
            }
        }
        return false;
    }

    void writeTree( boost::property_tree::ptree& pt ) const
    {
        using boost::property_tree::ptree;
        ptree ptAttr;
        if ( !proteinID.empty() ) ptAttr.add("protein_id", proteinID);
        if ( !locusTag.empty() ) ptAttr.add("locus_tag", locusTag);
        if ( !product.empty() ) ptAttr.add("product", product);
        if ( !refName.empty() ) ptAttr.add("ref_name", refName);

        // If the gene name is not available the product name will be used.
        // If the product name is also not available the refName will be used.
        pt.add("gene", gene.empty() ? (    product.empty() ? refName : boost::algorithm::to_lower_copy(product)
            ) : gene).push_back(make_pair("<xmlattr>",ptAttr));
    }
};

inline std::ostream& operator << ( std::ostream& os, Gene const& entry )
{
    return os << "gene      = " << entry.gene << std::endl
              << "locusTag  = " << entry.locusTag << std::endl
              << "proteinID = " << entry.proteinID << std::endl
              << "product   = " << entry.product << std::endl
              << "refName   = " << entry.refName << std::endl;
}

} // namespace GeneHunter

#endif /* GENE_H_ */
