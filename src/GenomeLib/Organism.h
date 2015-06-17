#ifndef ORGANISM_H_
#define ORGANISM_H_

#include "Gene.h"
#include "Variant.h"
#include <boost/filesystem/path.hpp>
#include <boost/property_tree/ptree.hpp>
#include <set>
#include <string>
#include <vector>

namespace GeneHunter {

class Organism
{
public:

    Organism();

    Organism( size_t taxonID, std::string const& taxonomy, std::string const& organismName,
        std::string const& genus, std::string const& species );

    size_t getTaxonID() const { return taxonID_; }
    std::string const& getTaxonomy() const { return taxonomy_; }
    std::string const& getOrganismName() const { return organismName_; }
    std::string const& getGenus() const { return genus_; }
    std::string const& getSpecies() const { return species_; }
    std::vector<std::string> const& getReads() const { return reads_; }

    void setTaxonID( size_t taxonID ) { taxonID_ = taxonID; }
    void setTaxonomy( std::string const& taxonomy ) { taxonomy_ = taxonomy; }
    void setOrganismName( std::string const& organismName ) { organismName_ = organismName; }
    void setGenus( std::string const& genus ) { genus_ = genus; }
    void setSpecies( std::string const& species ) { species_ = species; }

    void addRead( std::string const& read ) { reads_.push_back(read); }
    void addGene( Gene const& gene ) { genes_.insert(gene); }

    void writeTree( boost::property_tree::ptree& pt, size_t nbMatches );

    bool operator < ( Organism const& other ) const { return taxonID_ < other.taxonID_; }

    bool operator == ( Organism const& other ) const { return taxonID_ == other.taxonID_; }

private:

    size_t taxonID_ = 0;
    std::string taxonomy_;
    std::string organismName_;
    std::string genus_;
    std::string species_;

    std::set<Gene> genes_;
    std::vector<Variant> variants_;
    std::vector<std::string> reads_;

};

std::ostream& operator << ( std::ostream& os, Organism const& organism );

} // namespace GeneHunter

#endif /* ORGANISM_H_ */
