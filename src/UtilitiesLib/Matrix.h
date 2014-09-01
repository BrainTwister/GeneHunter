#ifndef MATRIX_H_
#define MATRIX_H_

#include <iomanip>
#include <iostream>
#include <fstream>
#include <vector>

template < class T >
struct RowProxy;

template < class T >
struct ConstRowProxy;

/**
 * Column-wise storage
 */
template < class T >
class Matrix
{
public:

	Matrix( size_t nbRows, size_t nbColumns )
	 : nbRows_(nbRows), nbColumns_(nbColumns), container_(nbRows*nbColumns)
	{}

	size_t nbRows() const { return nbRows_; }

	size_t nbColumns() const { return nbColumns_; }

	void resize( size_t nbRows, size_t nbColumns )
	{
		nbRows_ = nbRows;
		nbColumns_ = nbColumns;
		container_.resize(nbRows*nbColumns);
	}

	void setToZero()
	{
        for ( size_t i(0); i < nbColumns_; ++i )
		    for ( size_t j(0); j < nbRows_; ++j ) container_[i*nbRows_ + j] = 0.0;
	}

	T getMax() const
	{
		T max(0.0);

        for ( size_t i(0); i < nbRows_ * nbColumns_; ++i ) if ( container_[i] > max ) max = container_[i];

        return max;
	}

	std::pair<size_t,size_t> getMaxPosition() const
	{
		T max(0.0);
		size_t pos(0);

        for ( size_t i(0); i < nbRows_ * nbColumns_; ++i ) if ( container_[i] > max ) { max = container_[i]; pos = i; }

        return std::pair<size_t,size_t>(pos % nbRows_, pos / nbRows_);
	}

	RowProxy<T> operator [] ( size_t row )
	{
		return RowProxy<T>(*this,row);
	}

	ConstRowProxy<T> operator [] ( size_t row ) const
	{
		return ConstRowProxy<T>(*this,row);
	}

private:

	template < class T2 >
	friend class RowProxy;

	template < class T2 >
	friend class ConstRowProxy;

	size_t nbRows_;

	size_t nbColumns_;

	std::vector<T> container_;

};

template < class T >
struct RowProxy
{
	RowProxy( Matrix<T>& matrix, size_t row )
	 : matrix_(matrix), row_(row)
	{}

	RowProxy& operator = ( RowProxy const& ) = default;

	T& operator [] ( size_t column )
	{
		return matrix_.container_[row_ + column * matrix_.nbRows_];
	}

	Matrix<T>& matrix_;

	size_t row_;
};

template < class T >
struct ConstRowProxy
{
	ConstRowProxy( Matrix<T> const& matrix, size_t row )
	 : matrix_(matrix), row_(row)
	{}

	ConstRowProxy& operator = ( ConstRowProxy const& ) = default;

	T operator [] ( size_t column )
	{
		return matrix_.container_[row_ + column * matrix_.nbRows_];
	}

	Matrix<T> const& matrix_;

	size_t row_;
};

template < class T >
std::ostream& operator << ( std::ostream& os, Matrix<T> const& matrix )
{
	for ( size_t i(0); i != matrix.nbRows(); ++i ) {
		for ( size_t j(0); j != matrix.nbColumns(); ++j ) os << std::setw(2) << matrix[i][j] << " ";
		os << std::endl;
	}
    return os;
}

#endif /* MATRIX_H_ */
