#ifndef NUCLEOTIDEDATABASEITERATOR_H_
#define NUCLEOTIDEDATABASEITERATOR_H_

#include "FASTAIterator.h"
#include "GeneHunterException.h"
#include "NucleotideDatabase.h"
#include <boost/filesystem/path.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>
#include <fstream>

namespace GeneHunter {

template < class Traits >
class NucleotideDatabaseIterator
 : public boost::iterator_facade <
       NucleotideDatabaseIterator<Traits>,
   	   PtrNucleotideDatabase<Traits> const,
   	   boost::forward_traversal_tag
   >
{
public:

	NucleotideDatabaseIterator()
	 : ptrNucleotideDatabase()
	{}

	NucleotideDatabaseIterator( boost::filesystem::path const& filename, size_t maxNbEntries, size_t maxNbBases,
		size_t maxNbBasesPerFile, size_t startEntry, NucleotideDatabaseSettings const& settings
	)
	 : iterFASTACur(filename.string().c_str()),
	   iterFASTAEnd(),
	   maxNbEntries(maxNbEntries),
	   maxNbBases(maxNbBases),
	   maxNbBasesPerFile(maxNbBasesPerFile),
	   startEntry(startEntry),
	   settings(settings)
	{
		increment();
	}

private:

    friend class boost::iterator_core_access;

    PtrNucleotideDatabase<Traits> const& dereference() const
    {
        return ptrNucleotideDatabase;
    }

    bool equal( NucleotideDatabaseIterator const& other ) const
    {
        return ptrNucleotideDatabase == other.ptrNucleotideDatabase;
    }

    void increment()
    {
    	// Only construct a new database object if it is needed.
    	//if ( iterFASTACur != iterFASTAEnd and nbEntries != maxNbEntries and nbBases <= maxNbBases )
    		ptrNucleotideDatabase = PtrNucleotideDatabase<Traits>(new NucleotideDatabase<Traits>(settings));

    	size_t nbBasesPerFile(0);
    	for ( size_t entry(0); iterFASTACur != iterFASTAEnd; ++iterFASTACur, ++nbEntries, ++entry )
		{
    		if ( entry < startEntry ) continue;
			if ( nbEntries == maxNbEntries or nbBases > maxNbBases ) break;

    		ptrNucleotideDatabase->addEntry(*iterFASTACur);
    		nbBases += (*iterFASTACur)->getSequence().getNbBases();
    		nbBasesPerFile += (*iterFASTACur)->getSequence().getNbBases();

			//if ( ptrNucleotideDatabase->getMemoryUsage() > maxFileSize )
			if ( nbBasesPerFile > maxNbBasesPerFile )
			{
				++iterFASTACur;
				++nbEntries;
				return;
			}
		}

    	if ( nbBasesPerFile == 0 ) ptrNucleotideDatabase.reset();
    }

    FASTAIterator<typename Traits::RefSeqCharType> iterFASTACur;
    FASTAIterator<typename Traits::RefSeqCharType> iterFASTAEnd;

	PtrNucleotideDatabase<Traits> ptrNucleotideDatabase;

	NucleotideDatabaseSettings settings;

	size_t nbEntries = 0;
	size_t nbBases = 0;
	size_t startEntry = 0;

	size_t maxNbEntries;
    size_t maxNbBases;
    size_t maxNbBasesPerFile;

};

} // namespace GeneHunter

#endif /* NUCLEOTIDEDATABASEITERATOR_H_ */
