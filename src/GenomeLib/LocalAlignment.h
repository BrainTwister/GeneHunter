#ifndef LOCALALIGNMENT_H_
#define LOCALALIGNMENT_H_

#include "CreateDataClass.h"
#include "Sequence.h"

namespace GeneAssembler {

/**
 * \brief Local alignment using Smith-Waterman algorithm.
 */
class LocalAlignment {
public:

	CREATE_DATA_CLASS( Settings,\
		(( uint8_t, verbosity, 0 ))\
		(( bool, useBandMatrix, false ))\
		(( size_t, bandwidth, 0 ))\
		(( double, scoreMatch, 1.0 ))\
		(( double, scoreMismatch, -1.0 ))\
		(( double, scoreInsertion, -1.0 ))\
		(( double, scoreDeletion, -1.0 ))\
	)

	LocalAlignment( Settings const& settings = Settings() )
	 : settings(settings)
	{}

	double getQuality( Sequence<char> const& seq1, Sequence<char> const& seq2, Sequence<char> const& quality );

private:

	double getQualityDense( Sequence<char> const& seq1, Sequence<char> const& seq2, Sequence<char> const& quality );

	double getQualitySparse( Sequence<char> const& seq1, Sequence<char> const& seq2, Sequence<char> const& quality );

	double match( char c1, char c2 ) const;

	Settings settings;

};

inline double LocalAlignment::match( char c1, char c2 ) const
{
    if ( c1 == c2 ) return settings.scoreMatch_;
    else return settings.scoreMismatch_;
}

} // namespace GeneAssembler

#endif /* LOCALALIGNMENT_H_ */
