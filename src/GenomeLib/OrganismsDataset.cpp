#include "OrganismsDataset.h"
#include <boost/property_tree/xml_parser.hpp>

using namespace std;

namespace GeneHunter {

OrganismsDataset::OrganismsDataset( boost::filesystem::path const& datasetFile )
{
	using boost::property_tree::ptree;

	ptree pt;
	read_xml(datasetFile.string(),pt);
	ptree& ptChild = pt.get_child("organismsReport.dataset");

	datasetName_ = ptChild.get<string>("datasetName");
	hostGenus_ = ptChild.get<string>("hostGenus");
	hostSpecies_ = ptChild.get<string>("hostSpecies");
	commonName_ = ptChild.get<string>("commonName");
	taxonID_ = ptChild.get<size_t>("taxonomy.<xmlattr>.taxon_id");
	taxonomy_ = ptChild.get<string>("taxonomy");
	sampleSource_ = ptChild.get<string>("sampleSource");
	sampleType_ = ptChild.get<string>("sampleType");
	sequencerMethod_ = ptChild.get<string>("sequencer.<xmlattr>.method");
	sequencer_ = ptChild.get<string>("sequencer");
}

void OrganismsDataset::writeTree( boost::property_tree::ptree& pt )
{
	pt.put("datasetName", datasetName_);
	pt.put("hostGenus", hostGenus_);
	pt.put("hostSpecies", hostSpecies_);
	pt.put("commonName", commonName_);
	pt.put("taxonomy.<xmlattr>.taxon_id", taxonID_);
	pt.put("taxonomy", taxonomy_);
	pt.put("sampleSource", sampleSource_);
	pt.put("sampleType", sampleType_);
	pt.put("sequencer.<xmlattr>.method", sequencerMethod_);
	pt.put("sequencer", sequencer_);
}

} // namespace GeneHunter
