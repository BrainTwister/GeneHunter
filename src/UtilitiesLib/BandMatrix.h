#ifndef BANDMATRIX_H_
#define BANDMATRIX_H_

#include "FatalError.h"
#include <iomanip>
#include <iostream>
#include <fstream>
#include <vector>

/**
 * \brief BandMatrix
 *
 * - Column-wise storage
 * - Rectangular matrices are supported.
 * - A const object will return zero if the position is out of range, whereas a non-const object throw a FatalError.
 */
template < class T >
class BandMatrix
{
public:

    BandMatrix( size_t nbRows = 0, size_t nbColumns = 0, size_t leftBandwidth = 0, size_t rightBandwidth = 0 )
     : nbRows_(nbRows),
       nbColumns_(nbColumns),
       leftBandwidth_(leftBandwidth),
       rightBandwidth_(rightBandwidth),
       nbInternalColumns_(leftBandwidth + rightBandwidth + 1),
       container_(nbRows * nbInternalColumns_)
    {}

    struct RowProxy
    {
        RowProxy( BandMatrix& matrix, size_t row )
         : matrix_(matrix), row_(row)
        {}

        RowProxy& operator = ( RowProxy const& ) = default;

        T& operator [] ( size_t column ) const
        {
            size_t leftBorder = row_ > matrix_.leftBandwidth_ ? row_ - matrix_.leftBandwidth_ : 0;
            size_t rightBorder = matrix_.nbColumns_ - row_ > matrix_.rightBandwidth_ ? row_ + matrix_.rightBandwidth_ + 1 : matrix_.nbColumns_;
            GENEASSEMBLER_FATAL_ERROR(column < leftBorder or column >= rightBorder);
            return matrix_.container_[row_ + (column - leftBorder) * matrix_.nbRows_];
        }

        BandMatrix& matrix_;

        size_t row_;
    };

    struct ConstRowProxy
    {
        ConstRowProxy( BandMatrix const& matrix, size_t row )
         : matrix_(matrix), row_(row)
        {}

        ConstRowProxy& operator = ( ConstRowProxy const& ) = default;

        T operator [] ( size_t column ) const
        {
            size_t leftBorder = row_ > matrix_.leftBandwidth_ ? row_ - matrix_.leftBandwidth_ : 0;
            size_t rightBorder = matrix_.nbColumns_ - row_ > matrix_.rightBandwidth_ ? row_ + matrix_.rightBandwidth_ + 1 : matrix_.nbColumns_;
            if ( column < leftBorder or column >= rightBorder) return 0;
            else return matrix_.container_[row_ + (column - leftBorder) * matrix_.nbRows_];
        }

        BandMatrix const& matrix_;

        size_t row_;
    };

    size_t nbRows() const { return nbRows_; }

    size_t nbColumns() const { return nbColumns_; }

    size_t leftBandwidth() const { return leftBandwidth_; }

    size_t rightBandwidth() const { return rightBandwidth_; }

    void resize( size_t nbRows, size_t nbColumns, size_t leftBandwidth, size_t rightBandwidth )
    {
        nbRows_ = nbRows;
        nbColumns_ = nbColumns;
        leftBandwidth_ = leftBandwidth,
        rightBandwidth_ = rightBandwidth,
        nbInternalColumns_ = leftBandwidth_ + rightBandwidth_ + 1,
        container_.resize(nbRows_ * nbInternalColumns_);
    }

    void setToZero()
    {
        for ( size_t i(0); i < nbRows_ * nbInternalColumns_; ++i ) container_[i] = 0.0;
    }

    T getMax() const
    {
        T max(0.0);
        for ( size_t i(0); i < nbRows_ * nbInternalColumns_; ++i ) if ( container_[i] > max ) max = container_[i];
        return max;
    }

    RowProxy operator [] ( size_t row )
    {
        return RowProxy(*this,row);
    }

    ConstRowProxy operator [] ( size_t row ) const
    {
        return ConstRowProxy(*this,row);
    }

private:

    template < class T2 >
    friend std::ostream& operator << ( std::ostream& os, BandMatrix<T2> const& matrix );

    size_t nbRows_;

    size_t nbColumns_;

    size_t nbInternalColumns_;

    size_t leftBandwidth_;

    size_t rightBandwidth_;

    std::vector<T> container_;

};

template < class T >
std::ostream& operator << ( std::ostream& os, BandMatrix<T> const& matrix )
{
    for ( size_t i(0); i != matrix.nbRows_; ++i ) {
        size_t leftBorder = i > matrix.leftBandwidth_ ? (i - matrix.leftBandwidth_ > matrix.nbColumns_ ? matrix.nbColumns_ : i - matrix.leftBandwidth_) : 0;
        size_t rightBorder = matrix.nbColumns_ > i and matrix.nbColumns_ - i > matrix.rightBandwidth_ ? i + matrix.rightBandwidth_ + 1 : matrix.nbColumns_;
        for ( size_t j(0); j != leftBorder; ++j ) os << " x ";
        for ( size_t j(leftBorder); j != rightBorder; ++j ) os << std::setw(2) << matrix[i][j] << " ";
        for ( size_t j(rightBorder); j != matrix.nbColumns_; ++j ) os << " x ";
        os << std::endl;
    }
    return os;
}

#endif /* BANDMATRIX_H_ */
