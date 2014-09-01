#ifndef FASTQITERATOR_H_
#define FASTQITERATOR_H_

#include "FASTQ.h"
#include "GeneAssemblerException.h"
#include <boost/algorithm/string/trim.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/filesystem/path.hpp>
#include <fstream>

namespace GeneAssembler {

class FASTQIterator
 : public boost::iterator_facade <
   	   FASTQIterator,
   	   PtrFASTQ const,
   	   boost::forward_traversal_tag
   >
{
public:

	FASTQIterator()	{}

	FASTQIterator( boost::filesystem::path const& FASTQFilename )
	 : ptrInputStream_(new std::ifstream(FASTQFilename.string().c_str()))
	{
		increment();
	}

private:

    friend class boost::iterator_core_access;

    PtrFASTQ const& dereference() const
    {
        return ptrFASTQ_;
    }

    bool equal( FASTQIterator const& other ) const
    {
        return ptrFASTQ_ == other.ptrFASTQ_;
    }

    void increment()
    {
    	std::string line,name,sequence,quality;
    	bool headerFound = false;
    	bool baseQualitySeparatorFound = false;

		while ( std::getline(*ptrInputStream_,line) )
		{
			boost::algorithm::trim(line);
			if ( line.empty() ) continue;

			// Quality string can begin with '@'. To distinguish from header second query is needed.
			if ( line[0] == '@' and !headerFound ) {
				// Read header
	    		line.erase(0,1);
				name = std::move(line);
				headerFound = true;
			} else if ( line[0] == '+' and !baseQualitySeparatorFound ) {
				baseQualitySeparatorFound = true;
			} else {
				if (baseQualitySeparatorFound) {
					// Read quality sequence
					quality = std::move(line);
					break;
				} else {
					// Read base sequence
					sequence = std::move(line);
				}
			}
		}

    	if ( headerFound ) {
        	ptrFASTQ_ = PtrFASTQ(new FASTQ(name,sequence,quality));

    		if ( ptrFASTQ_->getQuality().getNbBases() != ptrFASTQ_->getSequence().getNbBases()
    			or ptrFASTQ_->getQuality().getNbBases() == 0 or ptrFASTQ_->getSequence().getNbBases() == 0 )
    		{
    			std::cout << "quality size  = " << ptrFASTQ_->getQuality().getNbBases() << std::endl;
    			std::cout << "sequence size = " << ptrFASTQ_->getSequence().getNbBases() << std::endl;
    			std::cout << "quality  = " << ptrFASTQ_->getQuality() << std::endl;
    			std::cout << "sequence = " << ptrFASTQ_->getSequence() << std::endl;
    			throw GeneAssemblerException("FASTQIterator: length of base and quality sequence are not equal or zero.");
    		}
    	} else {
    		ptrFASTQ_.reset();
    	}
    }

	boost::shared_ptr<std::ifstream> ptrInputStream_;

    PtrFASTQ ptrFASTQ_;

};

} // namespace GeneAssembler

#endif /* FASTQITERATOR_H_ */
