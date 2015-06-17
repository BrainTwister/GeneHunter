#ifndef FASTAITERATOR_H_
#define FASTAITERATOR_H_

#include "FASTA.h"
#include "Genome.h"
#include "GeneHunterException.h"
#include <boost/algorithm/string/trim.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/lexical_cast.hpp>
#include <fstream>

namespace GeneHunter {

template < class T >
class FASTAIterator
 : public boost::iterator_facade <
   	   FASTAIterator<T>,
   	   PtrFASTA<T> const,
   	   boost::forward_traversal_tag
   >
{
public:

	FASTAIterator() {}

	FASTAIterator( boost::filesystem::path const& filename )
	 : ptrInputStream_(new std::ifstream(filename.string().c_str()))
	{
		increment();
	}

	FASTAIterator( FASTAIterator const& other ) = default;

private:

    friend class boost::iterator_core_access;

    PtrFASTA<T> const& dereference() const
    {
        return ptrFASTA_;
    }

    bool equal( FASTAIterator const& other ) const
    {
        return ptrFASTA_ == other.ptrFASTA_;
    }

    void increment()
    {
    	ptrFASTA_ = PtrFASTA<T>(new FASTA<T>);

    	if ( lastHeaderLine_.empty() ) std::getline(*ptrInputStream_,lastHeaderLine_);

		ptrFASTA_->setGeneID(ptrFASTA_->getFirstGeneID(lastHeaderLine_));

    	std::string line;
    	std::string sequence;
		while ( std::getline(*ptrInputStream_,line) )
		{
			boost::algorithm::trim(line);
			if ( line[0] == '>' ) {
				lastHeaderLine_ = line;
				break;
			} else {
				// Read sequence
				sequence += line;
			}
		}

		if (sequence.empty()) ptrFASTA_.reset();
		else ptrFASTA_->setSequence(sequence);
    }

	boost::shared_ptr<std::ifstream> ptrInputStream_;

    PtrFASTA<T> ptrFASTA_;

    std::string lastHeaderLine_;

};

} // namespace GeneHunter

#endif /* FASTAITERATOR_H_ */
