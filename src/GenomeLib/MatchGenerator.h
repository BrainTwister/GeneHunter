#ifndef MATCHGENERATOR_H_
#define MATCHGENERATOR_H_

#include "NucleotideDatabase.h"

namespace GeneHunter {

CREATE_DATA_CLASS( MatchGeneratorSettings,\
    (( SearchAlgorithm, searchAlgorithm, FirstTokensAsSeed ))\
    (( size_t, maxSeeds, 300 ))\
    (( size_t, minTokensPerRead, 2 ))\
    (( size_t, maxBasesForVariants, 30 ))\
)

/**
 * Functor generating matches of \ref FASTQ read sequence on \ref NucleotideDatabase.
 * Thread-safe.
 */
struct MatchGenerator
{
    template < class Traits >
    Matches operator()( NucleotideDatabase<Traits> const& db, FASTQ const& read ) const;

    template < class Traits >
    void firstTokensAsSeed( NucleotideDatabase<Traits> const& db, FASTQ const& read ) const;

    template < class Traits >
    void takeMatchWithMostSeeds( NucleotideDatabase<Traits> const& db, FASTQ const& read ) const;
};

template < class Traits >
Matches operator()( NucleotideDatabase<Traits> const& db, FASTQ const& read ) const
{
    switch (settings.searchAlgorithm_) {
        case FirstTokensAsSeed :
            firstTokensAsSeed(db,read);
            break;
        case TakeMatchWithMostSeeds :
            takeMatchWithMostSeeds(db,read);
            break;
        default:
            throw GeneHunterException("MatchManager: unkown searchAlgorithm");
    }
}

} // namespace GeneHunter

#endif /* MATCHGENERATOR_H_ */
