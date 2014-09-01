#ifndef NUCLEOTIDEDATABASE_H_
#define NUCLEOTIDEDATABASE_H_

#include "CreateDataClass.h"
#include "GeneAssemblerException.h"
#include "Genome.h"
#include "FASTA.h"
#include "NucleotideDatabaseInformation.h"
#include "printHashTableState.h"
#include "SequenceToken.h"
#include "TokenIterator.h"
#include <boost/algorithm/string/trim.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/serialization/access.hpp>
#include "boost/serialization/unordered_map.hpp"
#include <boost/serialization/vector.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/tokenizer.hpp>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <unordered_map>

namespace GeneAssembler {

CREATE_DATA_CLASS( NucleotideDatabaseSettings,\
	(( size_t, initialBucketSize, 100000 ))\
	(( bool, createOnlyDatabaseInformation, false ))\
	(( bool, considerReverse, true ))\
	(( bool, considerComplement, true ))\
	(( bool, ignoreGeneIDsLargerThanMaxGeneID, false ))\
	(( size_t, maxGeneID, 0 ))\
)

template < class Traits >
class NucleotideDatabase
{
public:

	typedef typename Traits::HashTokenCharType HashTokenCharType;
	typedef typename Traits::RefSeqCharType RefSeqCharType;
	typedef typename Traits::HashTokenType HashTokenType;
	typedef PtrFASTA<RefSeqCharType> PtrRefEntry;

	struct ReferencePosition
	{
		ReferencePosition()
		 : ptrRefEntry(), position(0), reverse(false), complement(false)
		{}

		ReferencePosition( PtrRefEntry const& ptrRefEntry, size_t position, bool reverse, bool complement )
		 : ptrRefEntry(ptrRefEntry), position(position), reverse(reverse), complement(complement)
		{}

		PtrRefEntry ptrRefEntry;
		ReferenceSequencePositionType position;
		bool reverse;
		bool complement;

	    private:

	    friend class boost::serialization::access;

	    template < class Archive >
	    void serialize( Archive & ar, const unsigned int version )
	    {
	        ar & ptrRefEntry;
	        ar & position;
	        ar & reverse;
	        ar & complement;
	    }
	};

	typedef boost::shared_ptr<ReferencePosition> PtrReferencePosition;

private:

	typedef std::vector<PtrRefEntry> ReferenceDatabase;
	typedef std::unordered_multimap<HashTokenType,PtrReferencePosition,
		std::hash<HashTokenType>,std::equal_to<HashTokenType> > HashMapReferenceSequenceTokens;

public:

	NucleotideDatabase( NucleotideDatabaseSettings const& settings = NucleotideDatabaseSettings() )
	 : settings_(settings)
	{}

	// not copyable
	NucleotideDatabase( NucleotideDatabase const& other ) = delete;

	void printHashMapReferenceSequenceTokens( std::ostream& out ) const {
		for ( auto const& item : hashMapReferenceSequenceTokens_ ) {
			out << item.first << std::endl;
		}
	}

	void printMostFrequentOccurrences( std::ostream& out, size_t nb ) const
	{
		typedef std::unordered_map<HashTokenType,size_t,std::hash<HashTokenType>,std::equal_to<HashTokenType> > CountTokens;
		CountTokens countTokens;
		for ( auto const& item : hashMapReferenceSequenceTokens_ ) {
			typename CountTokens::iterator iterFind = countTokens.find(item.first);
			if (iterFind == countTokens.end()) countTokens.insert(std::make_pair(item.first,1));
			else ++iterFind->second;
		}

		std::multimap<size_t,HashTokenType,std::less<size_t> > nbTokens;
		for ( auto const& item : countTokens ) {
			nbTokens.insert(std::make_pair(item.second,item.first));
		}

		out << "nbTokens.size() = " << nbTokens.size() << std::endl;

		size_t counter(0);
		for ( auto const& item : nbTokens ) {
			if (++counter > nb) break;
			out << item << std::endl;
		}
	}

	size_t getMemoryUsage() const {
		return 0;
	}

	size_t getSizeOfHashMapReferenceSequenceTokens() const {
		return hashMapReferenceSequenceTokens_.size();
	}

	void printHashMapReferenceSequenceState( std::ostream& out ) const {
		printHashTableState(hashMapReferenceSequenceTokens_,out);
	}

	void printHashMapReferenceSequenceBucketEntries( std::ostream& out ) const {
		printHashTableBucketEntries(hashMapReferenceSequenceTokens_,out);
	}

	/**
	 * Return true if token was found in hash_map.
	 */
	bool find( HashTokenType const& token ) const
	{
		return hashMapReferenceSequenceTokens_.find(token) != hashMapReferenceSequenceTokens_.end();
	}

	void addEntry( PtrRefEntry const& ptrFASTA )
	{
		if (settings_.ignoreGeneIDsLargerThanMaxGeneID_ and ptrFASTA->getGeneID() > settings_.maxGeneID_) return;

		info_.addEntry(*ptrFASTA);
		if (settings_.createOnlyDatabaseInformation_) return;

		// Create token hash map
		for ( TokenIterator<HashTokenCharType,Traits::FixedTokenSize,RefSeqCharType> iterCur(ptrFASTA->getSequence()), iterEnd;
			iterCur != iterEnd; ++iterCur )
		{
			hashMapReferenceSequenceTokens_.insert(std::make_pair(iterCur->first,
			    PtrReferencePosition(new ReferencePosition(ptrFASTA,iterCur->second,false,false))));

			if (settings_.considerReverse_) {
				hashMapReferenceSequenceTokens_.insert(std::make_pair(reverse(iterCur->first),
					PtrReferencePosition(new ReferencePosition(ptrFASTA,
						ptrFASTA->getSequence().getNbBases() - iterCur->second - Traits::FixedTokenSize,true,false))));
			}

			if (settings_.considerComplement_) {
				hashMapReferenceSequenceTokens_.insert(std::make_pair(complement(iterCur->first),
					PtrReferencePosition(new ReferencePosition(ptrFASTA,iterCur->second,false,true))));
			}

			if (settings_.considerReverse_ and settings_.considerComplement_) {
				hashMapReferenceSequenceTokens_.insert(std::make_pair(reverseComplement(iterCur->first),
					PtrReferencePosition(new ReferencePosition(ptrFASTA,
						ptrFASTA->getSequence().getNbBases() - iterCur->second - Traits::FixedTokenSize,true,true))));
			}
		}
	}

	NucleotideDatabaseInformation const& getInformation() const { return info_; }

private:

    friend class boost::serialization::access;

    template < class Archive >
    void serialize( Archive & ar, const unsigned int version )
    {
        ar & hashMapReferenceSequenceTokens_;
        ar & settings_;
        ar & info_;
    }

	/// Storing position for each ReferenceSequenceToken.
	HashMapReferenceSequenceTokens hashMapReferenceSequenceTokens_;

    NucleotideDatabaseSettings settings_;

    NucleotideDatabaseInformation info_;

public:

	/**
	 * Return equal_range if token was found in hash_map.
	 * Abnormal position, because member have to be defined.
	 */
	auto getRange( HashTokenType const& token ) const -> decltype(hashMapReferenceSequenceTokens_.equal_range(token)) {
		return hashMapReferenceSequenceTokens_.equal_range(token);
	}
};

template < class Traits >
using PtrNucleotideDatabase = boost::shared_ptr<NucleotideDatabase<Traits> >;

} // namespace GeneAssembler

#endif /* NUCLEOTIDEDATABASE_H_ */
