#ifndef GENOME_H_
#define GENOME_H_

#include "CharTypes.h"
#include "SequenceToken.h"
#include "Version.h"
#include <string>

/**
 * \mainpage GeneAssembler
 *
 * The basic idea is to use fixed size sequence token to match the FASTQ reads
 * with the nucleotide reference database (NT). Fixed sized tokens will have the
 * advantage against flexible sized tokens that one-to-one matches between
 * reference and read tokens can be searched. Additionally it simplify the memory
 * management for the tokens. The fixed size (TokenSize in the following) has
 * tremendous influence to the efficiency of the algorithm. Therefore it will be a
 * compile-time variable, which was optimized one time and will be fixed at
 * run-time. The TokenSize should be large enough to hold the redundancy as small
 * as possible.
 */

namespace GeneAssembler {

/// Integer type for size of FixedToken.
typedef uint8_t FixedTokenSizeType;

template <
    FixedTokenSizeType Size,
    class HashChar = CompressedCharFourInOne,
	class RefChar = CompressedCharTwoInOne
>
struct Traits
{
	static const FixedTokenSizeType FixedTokenSize = Size;

	typedef HashChar HashTokenCharType;
	typedef RefChar RefSeqCharType;

	typedef SequenceToken<HashTokenCharType,FixedTokenSize> HashTokenType;
};

/// Integer type for NCBI GenBank identification number.
typedef uint32_t NuclGIType;

/// Integer type for position at which SequenceToken match the reference sequence.
typedef uint32_t ReferenceSequencePositionType;

/// Integer type for position at which SequenceToken match the read sequence.
typedef uint32_t ReadSequencePositionType;

/// Character type for read base sequence.
typedef char ReadSeqCharType;

/// Character type for read quality sequence.
typedef char ReadQualCharType;

/// Type for match quality.
typedef uint32_t QualityType;

/// Integer type for geneID number.
typedef uint32_t TaxonIDType;

} // namespace GeneAssembler

#endif /* GENOME_H_ */
