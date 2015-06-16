#ifndef NUCLEOTIDEDATABASEINFORMATION_H_
#define NUCLEOTIDEDATABASEINFORMATION_H_

#include "GeneHunterException.h"
#include "FASTA.h"
#include <boost/serialization/access.hpp>

namespace GeneHunter {

struct NucleotideDatabaseInformation
{
	NucleotideDatabaseInformation()
	 : nbImportedGeneSeq_(0), totalNbOfBases_(0), minGeneSeqSize_(std::numeric_limits<size_t>::max()), maxGeneSeqSize_(0)
	{}

	template < class Traits >
	void addEntry( FASTA<Traits> const& entry ) {
		++nbImportedGeneSeq_;
		size_t geneSeqSize = entry.getSequence().getNbBases();
		totalNbOfBases_ += geneSeqSize;
		if ( geneSeqSize < minGeneSeqSize_ ) minGeneSeqSize_ = geneSeqSize;
		if ( geneSeqSize > maxGeneSeqSize_ ) maxGeneSeqSize_ = geneSeqSize;
	}

	void add( size_t size ) {
		++nbImportedGeneSeq_;
		totalNbOfBases_ += size;
		if ( size < minGeneSeqSize_ ) minGeneSeqSize_ = size;
		if ( size > maxGeneSeqSize_ ) maxGeneSeqSize_ = size;
	}

	void merge( NucleotideDatabaseInformation const& other ) {
		nbImportedGeneSeq_ += other.nbImportedGeneSeq_;
		totalNbOfBases_ += other.totalNbOfBases_;
		minGeneSeqSize_ = std::min(minGeneSeqSize_,other.minGeneSeqSize_);
		maxGeneSeqSize_ = std::max(maxGeneSeqSize_,other.maxGeneSeqSize_);
	}

	size_t getNbImportedGeneSeq() const { return nbImportedGeneSeq_; }
	size_t getTotalNbOfBases() const { return totalNbOfBases_; }

private:

	friend std::ostream& operator << (std::ostream& os, NucleotideDatabaseInformation const& entry);

    friend class boost::serialization::access;

    template < class Archive >
    void serialize( Archive & ar, const unsigned int version )
    {
        ar & nbImportedGeneSeq_;
        ar & totalNbOfBases_;
        ar & minGeneSeqSize_;
        ar & maxGeneSeqSize_;
    }

	size_t nbImportedGeneSeq_;
	size_t totalNbOfBases_;
	size_t minGeneSeqSize_;
	size_t maxGeneSeqSize_;

};

std::ostream& operator << ( std::ostream& os, NucleotideDatabaseInformation const& entry );

} // namespace GeneHunter

#endif /* NUCLEOTIDEDATABASEINFORMATION_H_ */
