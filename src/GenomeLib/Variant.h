#ifndef VARIANT_H_
#define VARIANT_H_

#include "ProteinLink.h"
#include <boost/property_tree/ptree.hpp>
#include <string>

namespace GeneHunter {

//<sequenceDifference>
//  <variant ref_name="CAB54900.1" ref_start="44" ref_seq="-">A</variant>
//  <context offset="42">AGTAAAATCACGACTCTCANTATTTGCAAGCAAACCCGCCGAAAATTGCTGTAGGGCACNTTATTACCATGAGCATTAAGGCTATATTATTA</context>
//</sequenceDifference>

struct Variant
{
    Variant(
        std::string const& refName = "",
        size_t refStart = 0,
        std::string const& refSeg = "",
        std::string const& variant = "",
        size_t offset = 0,
        std::string const& context = ""
    ) :
        refName(refName),
        refStart(refStart),
        refSeg(refSeg),
        variant(variant),
        offset(offset),
        context(context)
    {}

    std::string refName;
    size_t refStart;
    std::string refSeg;
    std::string variant;
    size_t offset;
    std::string context;

    bool operator < ( Variant const& other ) const
    {
        return refName < other.refName or
               refStart < other.refStart or
               refSeg < other.refSeg or
               variant < other.variant or
               offset < other.offset or
               context < other.context;
    }

    void writeTree( boost::property_tree::ptree& pt ) const
    {
        using boost::property_tree::ptree;
        {
            ptree ptAttr;
            ptAttr.add("ref_name", refName);
            ptAttr.add("ref_start", refStart);
            ptAttr.add("ref_seg", refSeg);
            pt.add("variant", variant).push_back(make_pair("<xmlattr>",ptAttr));
        }
        {
            ptree ptAttr;
            ptAttr.add("offset", offset);
            pt.add("context", context).push_back(make_pair("<xmlattr>",ptAttr));
        }
    }
};

} // namespace GeneHunter

#endif /* VARIANT_H_ */
