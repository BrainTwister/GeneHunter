#include "BandMatrix.h"
#include "LocalAlignment.h"
#include "Matrix.h"
#include "Quality.h"

using namespace std;

namespace GeneHunter {

double LocalAlignment::getQuality( Sequence<char> const& seq1, Sequence<char> const& seq2, Sequence<char> const& quality )
{
	if ( settings.useBandMatrix_ and (seq1.size() + 1 > settings.bandwidth_ or seq2.size() + 1 > settings.bandwidth_))
		getQualitySparse(seq1,seq2,quality);
	else
	    getQualityDense(seq1,seq2,quality);
}

double LocalAlignment::getQualityDense( Sequence<char> const& seq1, Sequence<char> const& seq2, Sequence<char> const& quality )
{
	size_t nbRows = seq2.size() + 1;
	size_t nbColumns = seq1.size() + 1;

	// Similarity-score matrix, seq1 are columns, seq2 are rows
	Matrix<double> H(nbRows,nbColumns);
    H.setToZero();

	for ( size_t i(1); i < nbRows; ++i ) {
		for ( size_t j(1); j < nbColumns; ++j ) {
			H[i][j] = std::max(0.0, std::max(H[i-1][j-1] + match(seq2[i-1],seq1[j-1]) * (1.0 - FastqSanger::data[quality[j-1]]),
				std::max(H[i-1][j] + settings.scoreDeletion_, H[i][j-1] + settings.scoreInsertion_)));
		}
	}

	if (settings.verbosity_ >= 3) cout << H << endl;

	return H.getMax();
}

double LocalAlignment::getQualitySparse( Sequence<char> const& seq1, Sequence<char> const& seq2, Sequence<char> const& quality )
{
	size_t nbRows = seq2.size() + 1;
	size_t nbColumns = seq1.size() + 1;

	// Similarity-score matrix, seq1 are columns, seq2 are rows
	BandMatrix<double> H(nbRows,nbColumns,settings.bandwidth_,settings.bandwidth_);
    H.setToZero();

    // const reference for reading matrix returns zero if BandMatrix position is out of bandwidth.
    BandMatrix<double> const& cH = H;

	for ( size_t i(1); i < nbRows; ++i ) {
		size_t leftBorder = i > H.leftBandwidth() ? (i - H.leftBandwidth() > H.nbColumns() ? H.nbColumns() : i - H.leftBandwidth()) : 0;
		size_t rightBorder = H.nbColumns() > i and H.nbColumns() - i > H.rightBandwidth() ? i + H.rightBandwidth() + 1 : H.nbColumns();
		for ( size_t j(leftBorder); j != rightBorder; ++j ) {
			H[i][j] = std::max(0.0, std::max(cH[i-1][j-1] + match(seq2[i-1],seq1[j-1]) * (1.0 - FastqSanger::data[quality[j-1]]),
				std::max(cH[i-1][j] + settings.scoreDeletion_, cH[i][j-1] + settings.scoreInsertion_)));
		}
	}

	if (settings.verbosity_ >= 3) cout << H << endl;

	return H.getMax();
}

} // namespace GeneHunter
