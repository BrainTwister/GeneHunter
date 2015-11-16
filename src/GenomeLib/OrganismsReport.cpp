#include "GenomeLib/Match.h"
#include "GenomeLib/OrganismsReport.h"
#include "UtilitiesLib/FileIO.h"
#include "UtilitiesLib/GeneHunterException.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

using namespace std;

namespace GeneHunter {

OrganismsReport::OrganismsReport(filesystem::path const& reportFile, filesystem::path const& datasetFile,
    Settings const& settings, size_t nbReads)
 : dataset(datasetFile),
   reportFile(reportFile),
   taxonomy(settings.taxonomySettings_),
   nbReads(nbReads),
   cdsDatabase(settings.cdsDatabaseSettings_),
   settings(settings)
{
    PtrOrganism ptrOrganism(new Organism(dataset.getTaxonID(),dataset.getTaxonomy(),"Host","",""));
    organisms.push_back(ptrOrganism);
    organismLookupTable.insert(make_pair(dataset.getTaxonID(),ptrOrganism));
}

OrganismsReport::~OrganismsReport()
{
    using boost::property_tree::ptree;

    ptree pt;
    ptree& ptReport = pt.add_child("organismsReport",pt);

    ptree ptDataset;
    dataset.writeTree(ptDataset);
    ptReport.push_back(make_pair("dataset",ptDataset));

    // Sort organisms by number of reads.
    std::sort(organisms.begin(),organisms.end(),[]( PtrOrganism const& p1, PtrOrganism const& p2 ) -> bool
        { return p1->getReads().size() > p2->getReads().size(); });

    ptree ptOrganisms, ptRemovedOrganisms;
    for ( auto elem : organisms )
    {
        // Filter organism which must not be reported.
        if ( elem->getOrganismName() == "Host" or
               ( settings.removeUnwantedOrganisms_ and (
                     elem->getTaxonomy().substr(0,15) == "other sequences" or
                   ( elem->getTaxonomy().substr(0,9) == "Eukaryota" and elem->getTaxonomy().substr(11,5) != "Fungi" ) or
                     elem->getTaxonomy() == "Bacteria; environmental samples." )
               ) or
             elem->getReads().size() < settings.minReadsForOrganism_ )
        {
            ptree ptOrganism;
            elem->writeTree(ptOrganism,nbReads);
            ptRemovedOrganisms.push_back(make_pair("organism",ptOrganism));
        } else {
            ptree ptOrganism;
            elem->writeTree(ptOrganism,nbReads);
            ptOrganisms.push_back(make_pair("organism",ptOrganism));
        }
    }
    ptReport.push_back(make_pair("organisms",ptOrganisms));

    std::ofstream output(reportFile.string().c_str());
    write_xml(output,pt,boost::property_tree::xml_writer_settings<char>(' ',2));

    std::ofstream removedOrganismsOut("removedOrganisms.xml");
    write_xml(removedOrganismsOut,ptRemovedOrganisms,boost::property_tree::xml_writer_settings<char>(' ',2));

    if ( !geneIDsNotFountInTaxonomy.empty() )
    {
        if ( geneIDsNotFountInTaxonomy.size() == 1 ) cout << "For one geneID was " << endl;
        else cout << "For " << geneIDsNotFountInTaxonomy.size() << " geneIDs were " << endl;
        cout << "no taxonomy entry found (see file geneIDsNotFountInTaxonomy.txt)." << endl;

        std::ofstream output2("geneIDsNotFountInTaxonomy.txt");
        output2 << "# List of geneIDs for which no taxonomy entry were found." << endl;
        for ( auto const& elem : geneIDsNotFountInTaxonomy ) output2 << elem << endl;
    }
}

void OrganismsReport::addMatches( Matches const& matches )
{
    size_t nbAddedMatches(0);
    if ( settings.verbosity_ >= 1 ) cout << "Number of matches to add = " << matches.size() << endl;

    for ( auto const& elem : matches )
    {
        auto match = elem.second;

        if ( match.nuclGI_ == 0 )
            throw GeneHunterException("OrganismsReport::addMatches: geneID == 0");

        Organism organism;
        try {
            organism = taxonomy.getOrganism(match.nuclGI_);
        } catch ( ... ) {
            geneIDsNotFountInTaxonomy.insert(match.nuclGI_);
            continue;
        }

        PtrOrganism ptrOrganism;
        auto iterFind = organismLookupTable.find(organism.getTaxonID());
        if ( iterFind == organismLookupTable.end() ) {
            ptrOrganism = PtrOrganism(new Organism(organism));
              organisms.push_back(ptrOrganism);
            organismLookupTable.insert(std::make_pair(ptrOrganism->getTaxonID(),ptrOrganism));
        } else {
            ptrOrganism = iterFind->second;
        }

        if ( ptrOrganism->getOrganismName() != "Host" )
        {
            ptrOrganism->addRead(elem.first);

            for ( auto const& gene : cdsDatabase.getGene(match.nuclGI_,match.refStart_,match.refEnd_) )
                ptrOrganism->addGene(gene);

            //ptrOrganism->addVariants(cdsDatabase.getVariant(match));
        }

        ++nbAddedMatches;
        if (!(nbAddedMatches % settings.matchNotificationNumber_) and ( settings.verbosity_ >= 1 ))
            cout << "Number of matches added so far = " << nbAddedMatches << endl;
    }
}

} // namespace GeneHunter
