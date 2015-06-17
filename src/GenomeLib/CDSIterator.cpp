#include "CDSIterator.h"

using namespace std;

namespace GeneHunter {

size_t CDSIterator::indention_ = 21;

void CDSIterator::increment()
{
    currentEntry_.clearWithoutGeneID();

    if ( settings_.onlyImportKeyFeaturesWithPrecedingGeneSection_ )    {
        bool precedingGeneSectionFound(false);
        while ( std::getline(*ptrInputStream_,currentLine_) )
        {
            if ( currentLine_.find("VERSION") != std::string::npos and currentLine_.substr(0,7) == "VERSION" ) {
                try { readGeneID(); } catch ( ... ) {}
            } else if ( currentLine_.find("gene") != std::string::npos and currentLine_.substr(5,4) == "gene" ) {
                precedingGeneSectionFound = true;
            } else if ( currentLineContainFeatureKey() and precedingGeneSectionFound ) {
                try { readCDS(); return; } catch ( ... ) {}
                precedingGeneSectionFound = false;
            }
        }
    } else {
        while ( std::getline(*ptrInputStream_,currentLine_) )
        {
            if ( currentLine_.find("VERSION") != std::string::npos and currentLine_.substr(0,7) == "VERSION" ) {
                try { readGeneID(); } catch ( ... ) {}
            } else if ( currentLineContainFeatureKey() ) {
                try { readCDS(); return; } catch ( ... ) {
                    cerr << "Error reading CDS" << endl;
                }
            }
        }
    }
    endFlag_ = true;
}

void CDSIterator::readGeneID()
{
    size_t pos = currentLine_.find("GI:");
    if ( pos == std::string::npos ) {
        std::cerr << "CDSIterator::readGeneID: GI not found in VERSION line" << std::endl
                  << "line: " << currentLine_ << std::endl;
        throw GeneHunterException("CDSIterator::readGeneID: GI not found in VERSION line");
    }

    currentEntry_.geneID_ = boost::lexical_cast<size_t>(currentLine_.substr(pos+3,currentLine_.size()));
    currentEntry_.referenceName_ = boost::algorithm::trim_copy(currentLine_.substr(8,pos-8));
}

void CDSIterator::readCDS()
{
    currentEntry_.location_ = boost::algorithm::trim_copy(currentLine_.substr(indention_,currentLine_.size()));
    CurrentReading currentReading = Location;

    while ( std::getline(*ptrInputStream_,currentLine_) )
    {
        // If anything other than whitespaces are found within indention the CDS section is closed.
        if ( !boost::algorithm::trim_copy(currentLine_.substr(0,indention_)).empty() ) {
            removeQuotationMarks(currentEntry_.ptrProteinLink_->locusTag_);
            removeQuotationMarks(currentEntry_.ptrProteinLink_->gene_);
            removeQuotationMarks(currentEntry_.ptrProteinLink_->proteinID_);
            removeQuotationMarks(currentEntry_.ptrProteinLink_->product_);
            return;
        }

        if ( currentLine_.substr(indention_,1) == "/" )
        {
            size_t pos = currentLine_.find("=");
            if ( pos == std::string::npos ) {
                currentReading = Unused;
                continue;
            }

            std::string name = currentLine_.substr(indention_+1,pos-indention_-1);

            if ( name == "locus_tag" ) {
                currentEntry_.ptrProteinLink_->locusTag_ = boost::algorithm::trim_copy(currentLine_.substr(pos+1,currentLine_.size()));
                currentReading = LocusTag;
            } else if ( name == "gene" ) {
                currentEntry_.ptrProteinLink_->gene_ = boost::algorithm::trim_copy(currentLine_.substr(pos+1,currentLine_.size()));
                currentReading = Gene;
            } else if ( name == "protein_id" ) {
                currentEntry_.ptrProteinLink_->proteinID_ = boost::algorithm::trim_copy(currentLine_.substr(pos+1,currentLine_.size()));
                currentReading = ProteinID;
            } else if ( name == "product" ) {
                currentEntry_.ptrProteinLink_->product_ = boost::algorithm::trim_copy(currentLine_.substr(pos+1,currentLine_.size()));
                currentReading = Product;
            } else {
                currentReading = Unused;
            }
        } else {
            if ( currentReading == Location )
                currentEntry_.location_ += boost::algorithm::trim_copy(currentLine_.substr(indention_,currentLine_.size()));
            else if ( currentReading == Gene )
                currentEntry_.ptrProteinLink_->gene_ += boost::algorithm::trim_copy(currentLine_.substr(indention_,currentLine_.size()));
            else if ( currentReading == LocusTag )
                currentEntry_.ptrProteinLink_->locusTag_ += boost::algorithm::trim_copy(currentLine_.substr(indention_,currentLine_.size()));
            else if ( currentReading == ProteinID )
                currentEntry_.ptrProteinLink_->proteinID_ += boost::algorithm::trim_copy(currentLine_.substr(indention_,currentLine_.size()));
            else if ( currentReading == Product )
                currentEntry_.ptrProteinLink_->product_ += boost::algorithm::trim_copy(currentLine_.substr(indention_,currentLine_.size()));
            else if ( currentReading != Unused )
                throw GeneHunterException("CDSIterator::readCDS: Unkown currentReading");
        }
    }
}

void CDSIterator::removeQuotationMarks( std::string& str ) const
{
    if( !str.empty() ) {
        str.erase(str.begin());
        str.erase(str.end()-1);
    }
    str.shrink_to_fit();
}

bool CDSIterator::currentLineContainFeatureKey() const
{
    for ( auto const& key : settings_.listOfFeatureKeysToImport_ )
        if (currentLine_.find(key) != std::string::npos and currentLine_.substr(5,key.size()) == key) return true;
    return false;
}

} // namespace GeneHunter
