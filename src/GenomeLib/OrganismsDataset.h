#ifndef ORGANISMSDATASET_H_
#define ORGANISMSDATASET_H_

#include <boost/filesystem/path.hpp>
#include <boost/property_tree/ptree.hpp>
#include <string>

namespace GeneHunter {

class OrganismsDataset
{
public:

	OrganismsDataset( boost::filesystem::path const& datasetFile );

    void writeTree( boost::property_tree::ptree& pt );

	size_t getTaxonID() const { return taxonID_; }
	std::string getTaxonomy() const { return taxonomy_; }

private:

	std::string datasetName_;
	std::string hostGenus_;
	std::string hostSpecies_;
	std::string commonName_;
	size_t taxonID_;
	std::string taxonomy_;
	std::string sampleSource_;
	std::string sampleType_;
	std::string sequencerMethod_;
	std::string sequencer_;

};

} // namespace GeneHunter

#endif /* ORGANISMSDATASET_H_ */
