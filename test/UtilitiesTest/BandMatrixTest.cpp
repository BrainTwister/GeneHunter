#include "UtilitiesLib/BandMatrix.h"
#include <gtest/gtest.h>
#include <exception>

TEST(BandMatrixTest,Case0)
{
    BandMatrix<double> A(3,3,1,1);
    A.setToZero();
    A[2][1] = 7.0;

    EXPECT_EQ(7.0, A[2][1]);
    EXPECT_THROW(A[2][0], std::exception);
}

TEST(BandMatrixTest,Case1)
{
    BandMatrix<double> A(5,5,1,1);
    A.setToZero();
    A[2][1] = 2.0;

    EXPECT_DOUBLE_EQ(2.0, A[2][1]);
    EXPECT_THROW(A[2][0], std::exception);
    EXPECT_THROW(A[0][2], std::exception);
}

TEST(BandMatrixTest,Case2)
{
    BandMatrix<double> A(3,5,1,1);
    A.setToZero();
    A[2][1] = 2.0;

    EXPECT_DOUBLE_EQ(2.0, A[2][1]);
    EXPECT_THROW(A[2][0], std::exception);
    EXPECT_THROW(A[0][2], std::exception);
}

TEST(BandMatrixTest,Case3)
{
    BandMatrix<double> A(10,7,2,2);
    A.setToZero();
    A[2][1] = 2.0;

    EXPECT_DOUBLE_EQ(2.0, A[2][1]);
    EXPECT_THROW(A[3][0], std::exception);
    EXPECT_THROW(A[0][3], std::exception);
}
