#include "Organism.h"
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <sstream>

using namespace std;

namespace GeneHunter {

Organism::Organism()
 : taxonID_(0)
{}

Organism::Organism( size_t taxonID, std::string const& taxonomy, std::string const& organismName,
	std::string const& genus, std::string const& species )
 : taxonID_(taxonID),
   taxonomy_(taxonomy),
   organismName_(organismName),
   genus_(genus),
   species_(species)
{}

void Organism::writeTree( boost::property_tree::ptree& pt, size_t nbReads )
{
	using boost::property_tree::ptree;

	pt.put("relativeAmount.<xmlattr>.count", reads_.size());

	stringstream ssRelativeAmount;
	ssRelativeAmount << fixed << setprecision(4);
	if (nbReads) ssRelativeAmount << (static_cast<double>(reads_.size())/nbReads);
	else ssRelativeAmount << 0.0;

	pt.put("relativeAmount", ssRelativeAmount.str());
	pt.put("taxonomy.<xmlattr>.taxon_id", taxonID_);
	pt.put("taxonomy", taxonomy_);
	pt.put("organismName", organismName_);

	if ( !genus_.empty() ) pt.put("genus", genus_);
	if ( !species_.empty() ) pt.put("species", species_);

	if ( !genes_.empty() and organismName_ != "Host" ) {
		ptree ptGenes;
		for ( auto const& gene : genes_ ) {
			gene.writeTree(ptGenes);
		}
		pt.push_back(make_pair("genes",ptGenes));
	}

	if ( !variants_.empty() and organismName_ != "Host" ) {
		ptree ptVariant;
		for ( auto const& variant : variants_ ) variant.writeTree(ptVariant);
		pt.push_back(make_pair("sequenceDifference",ptVariant));
	}

	if ( !reads_.empty() and organismName_ != "Host" ) {
		ptree ptReads;
		for ( auto const& read : reads_ ) ptReads.add("sequence",read);
		pt.push_back(make_pair("reads",ptReads));
	}
}

std::ostream& operator << ( std::ostream& os, Organism const& organism )
{
	return os << "taxonID       = " << organism.getTaxonID() << endl
			  << "taxonomy      = " << organism.getTaxonomy() << endl
			  << "organismName  = " << organism.getOrganismName() << endl
			  << "genus         = " << organism.getGenus() << endl
			  << "species       = " << organism.getSpecies() << endl
	          << "reads.size()  = " << organism.getReads().size() << endl;
}

} // namespace GeneHunter
