#ifndef FULLMATCHMANAGER_H_
#define FULLMATCHMANAGER_H_

#include "CreateDataClass.h"
#include "FASTA.h"
#include "FASTQ.h"
#include "Genome.h"
#include "Helpers.h"
#include "LocalAlignment.h"
#include "Match.h"
#include "NucleotideDatabase.h"
#include "Quality.h"
#include "Range.h"
#include "Settings.h"
#include <boost/serialization/access.hpp>
#include <boost/serialization/version.hpp>
#include <map>
#include <set>

namespace GeneAssembler {

template < class T1, class T2 >
auto begin( std::pair<T1,T2> p) -> T1 { return p.first; }

template < class T1, class T2 >
auto end( std::pair<T1,T2> p) -> T2 { return p.second; }

CREATE_DATA_CLASS( FullMatchManagerSettings,\
	(( SearchAlgorithm, searchAlgorithm, FirstTokensAsSeed ))\
	(( size_t, maxSeeds, 100 ))\
	(( size_t, minTokensPerRead, 3 ))\
	(( size_t, maxBasesForVariants, 8 ))\
	(( LocalAlignment::Settings, localAlignmentSettings, LocalAlignment::Settings() ))\
)

class FullMatchManager
{
public:

	FullMatchManager( FullMatchManagerSettings const& settings = FullMatchManagerSettings() )
	 : settings(settings), totalNbImportedReadSeq(0)
	{}

	/// Include match if it is not default.
	void addMatch( std::string const& readName, Match const& match );

	/// Return match if found, otherwise return default match.
	Match getMatch( std::string const& readName ) const;

	/// Return all matches.
	Matches const& getMatches() const { return matches; }

	template < class Traits >
	void findBestMatch( NucleotideDatabase<Traits> const& db, FASTQ const& read, Match& match ) const;

	template < class Traits >
	void includeBestMatch( NucleotideDatabase<Traits> const& db, FASTQ const& read );

	size_t getTotalNbImportedReadSeq() const { return totalNbImportedReadSeq; }

	void setTotalNbImportedReadSeq( size_t nb ) { totalNbImportedReadSeq = nb; }

private:

	template < class Traits >
	void firstTokensAsSeed( NucleotideDatabase<Traits> const& db, FASTQ const& read, Match& match ) const;

	template < class Traits >
	void takeMatchWithMostSeeds( NucleotideDatabase<Traits> const& db, FASTQ const& read, Match& match ) const;

	template < class Iterator1, class Iterator2 >
	QualityType getQuality( Iterator1 iterRefCur, Iterator1 const& iterRefEnd,
		Iterator2 iterReadCur, Iterator2 iterQualityCur, QualityType threshold ) const
	{
		QualityType quality = 0;
	    for ( ; iterRefCur != iterRefEnd; ++iterRefCur, ++iterReadCur, ++iterQualityCur ) {
	        if ( *iterRefCur != *iterReadCur ) quality += *iterQualityCur - FastqSanger::offset;
	        if ( quality > threshold ) break;
	    }
	    return quality;
	}

    friend class boost::serialization::access;

    template < class Archive >
    void serialize( Archive& ar, const unsigned int version ) {
        ar & BOOST_SERIALIZATION_NVP(matches);
        if (version > 0) ar & BOOST_SERIALIZATION_NVP(totalNbImportedReadSeq);
    }

    FullMatchManagerSettings settings;

	Matches matches;

	size_t totalNbImportedReadSeq;

};

template < class Traits >
void FullMatchManager::findBestMatch( NucleotideDatabase<Traits> const& db, FASTQ const& read, Match& curMatch ) const
{
	switch (settings.searchAlgorithm_) {
	    case FirstTokensAsSeed :
	    	firstTokensAsSeed(db,read,curMatch);
	    	break;
	    case TakeMatchWithMostSeeds :
            takeMatchWithMostSeeds(db,read,curMatch);
            break;
	    default:
	    	throw GeneAssemblerException("FullMatchManager: unkown searchAlgorithm");
	}
}

template < class Traits >
void FullMatchManager::includeBestMatch( NucleotideDatabase<Traits> const& db, FASTQ const& read )
{
	Match match = getMatch(read.getName());
	findBestMatch(db,read,match);
	addMatch(read.getName(),match);
}

template < class Traits >
void FullMatchManager::firstTokensAsSeed( NucleotideDatabase<Traits> const& db, FASTQ const& read, Match& curMatch ) const
{
	using namespace std;

	typedef typename Traits::HashTokenCharType HashTokenCharType;
	typedef typename Traits::RefSeqCharType RefSeqCharType;

	// TODO: improve token selection, not only the from beginning
	size_t count = 0;
	for ( TokenIterator<HashTokenCharType,Traits::FixedTokenSize,ReadSeqCharType,1> iterTokenCur(read.getSequence()), iterTokenEnd;
		iterTokenCur != iterTokenEnd; ++iterTokenCur, ++count )
	{
		if ( count == settings.maxSeeds_ ) break;

		for ( auto const& match : db.getRange(iterTokenCur->first) )
		{
			auto ptrRefEntry = match.second->ptrRefEntry;
			auto refPos = match.second->position;
			auto readPos = iterTokenCur->second;

			if ( refPos < readPos ) continue;

			size_t refStart = refPos - readPos;
			size_t refEnd = refStart + read.getSequence().getNbBases();

			if ( refEnd > ptrRefEntry->getSequence().getNbBases() ) continue;

			Sequence<ReadSeqCharType> refSeq(ptrRefEntry->getSequence());

			QualityType quality = getQuality(
				refSeq.begin() + refStart,
				refSeq.begin() + refEnd,
				read.getSequence().begin(),
				read.getQuality().begin(),
				curMatch.quality_
			);

			if ( quality < curMatch.quality_ ) {
				curMatch.nuclGI_ = ptrRefEntry->getGeneID();
				curMatch.refStart_ = refStart;
				curMatch.refEnd_ = refEnd;
				curMatch.quality_ = quality;
			}
		}
	}
}

template < class Traits >
void FullMatchManager::takeMatchWithMostSeeds( NucleotideDatabase<Traits> const& db, FASTQ const& read, Match& curMatch ) const
{
	typedef typename NucleotideDatabase<Traits>::PtrRefEntry PtrRefEntry;
	typedef typename Traits::HashTokenCharType HashTokenCharType;
	typedef typename Traits::RefSeqCharType RefSeqCharType;
	typedef std::tuple<ReferenceSequencePositionType,bool,bool> Position;
	typedef typename std::map<Position,size_t> PositionCountList;
	typedef typename std::unordered_map<PtrRefEntry,PositionCountList> MatchLookup;

	// matchLookup collect all found tokens and sort them by FASTA entry.
	MatchLookup matchLookup;
	PtrRefEntry ptrRefEntry;
	size_t count(0);
	size_t maxNbTokens(0);

	for ( TokenIterator<HashTokenCharType,Traits::FixedTokenSize,ReadSeqCharType,1> iterTokenCur(read.getSequence()), iterTokenEnd;
		iterTokenCur != iterTokenEnd; ++iterTokenCur, ++count )
	{
		if ( count == settings.maxSeeds_ ) break;

		for ( auto const& match : db.getRange(iterTokenCur->first) )
		{
			ptrRefEntry = match.second->ptrRefEntry;
			size_t refPos = match.second->position;
			size_t readPos = iterTokenCur->second;

			// First position of read must not be before the first position of reference.
			if ( readPos > refPos ) continue;

			size_t refStart = refPos - readPos;
			size_t refEnd = refStart + read.getSequence().getNbBases();

			// Last position of read must not be behind the last position of reference.
			if ( refEnd > ptrRefEntry->getSequence().getNbBases() ) continue;

			auto iterEntry = matchLookup.find(ptrRefEntry);
			if ( iterEntry == matchLookup.end() ) {
				PositionCountList positionCountList;
				if ( maxNbTokens < 1 ) maxNbTokens = 1;
				positionCountList.insert(std::make_pair(std::make_tuple(refStart,match.second->reverse,match.second->complement),1));
				matchLookup.insert(std::make_pair(ptrRefEntry,positionCountList));
			} else {

				// - If geneID/refStart pair was not found in matchLookup within the maxBasesForVariants range
				//   create a new one and set counter to 1.
				// - If geneID/refStart pair exist and is identical to refStart increment counter.
				// - If geneID/refStart pair exist but is not identical to refStart create a new entry,
				//   set counter to 2, and increment the entry was found in MatchLookup.

				PositionCountList tmpPositionCountList;
				bool found = false;
				for ( auto & elem : iterEntry->second ) {
					size_t refStartInCountList = std::get<0>(elem.first);
				    if ( (std::max(refStartInCountList,refStart) - std::min(refStartInCountList,refStart)) <= settings.maxBasesForVariants_) {
						++elem.second;
						found = true;
						if ( maxNbTokens < elem.second ) maxNbTokens = elem.second;
					    if ( std::get<0>(elem.first) != refStart ) {
					    	tmpPositionCountList.insert(std::make_pair(std::make_tuple(refStartInCountList,match.second->reverse,match.second->complement),2));
					    }
					    else break;
					}
				}
				if (!found) {
					if ( maxNbTokens < 1 ) maxNbTokens = 1;
					iterEntry->second.insert(std::make_pair(std::make_tuple(refStart,match.second->reverse,match.second->complement),1));
				}
				else iterEntry->second.insert(tmpPositionCountList.begin(),tmpPositionCountList.end());
			}
		}
	}

	//std::cout << "matchLookup = " << matchLookup << std::endl;
	//std::cout << "maxNbTokens = " << maxNbTokens << std::endl;

	if ( maxNbTokens < curMatch.nbTokenMatches_ or maxNbTokens < settings.minTokensPerRead_ ) return;

	for ( auto const& entry : matchLookup ) {
		for ( auto const& position : entry.second ) {

		    if ( position.second < maxNbTokens ) continue;

		    size_t refLength = entry.first->getSequence().getNbBases();

			size_t refStart,refEnd;
			if ( std::get<1>(position.first) ) { // reverse?
			    refEnd = refLength - std::get<0>(position.first);
			    refStart = refEnd - read.getSequence().getNbBases();
			} else {
			    refStart = std::get<0>(position.first);
			    refEnd = refStart + read.getSequence().getNbBases();
			}

			Sequence<ReadSeqCharType> refSeq(entry.first->getSequence().getString().substr(refStart,read.getSequence().getNbBases()));

			if ( std::get<1>(position.first) ) { // reverse?
				refSeq = reverse(refSeq);
			}

			if ( std::get<2>(position.first) ) { // complement?
				refSeq = complement(refSeq);
			}

			LocalAlignment localAlignment(settings.localAlignmentSettings_);
			auto quality = localAlignment.getQuality(read.getSequence(),refSeq,read.getQuality());

			//std::cout << "result.quality_ = " << result.quality_ << std::endl;

			if ( quality > curMatch.quality_ ) {
				curMatch.nuclGI_ = entry.first->getGeneID();
				curMatch.refStart_ = refStart;
				curMatch.refEnd_ = refEnd;
				curMatch.quality_ = quality;
				curMatch.nbTokenMatches_ = maxNbTokens;
			}
		}
	}
}

} // namespace GeneAssembler

BOOST_CLASS_VERSION(GeneAssembler::FullMatchManager,1)

#endif /* FULLMATCHMANAGER_H_ */
