#include "BandMatrix.h"
#include "gtest/gtest.h"

using namespace std;

TEST(BandMatrixTest,TestAccess)
{
	BandMatrix<double> A(3,3,1,1);
	A[2][1] = 7.0;

	EXPECT_EQ(7.0,A[2][1]);
	EXPECT_THROW(A[2][0],std::exception);
}

#if 0
{
	BandMatrix<double> A(5,5,1,1);
	A.setToZero();

	A[2][1] = 2.0;

	if ( A[2][1] != 2.0 ) testSuite.errorLog(name() + ": wrong element 2/1");

	try {
		A[2][0] = 3.0;
		testSuite.errorLog(name() + ": forbidden write access 2/1");
	} catch ( std::exception const& e ) {}

	try {
		A[0][2] = 3.0;
		testSuite.errorLog(name() + ": forbidden write access 2/2");
	} catch ( std::exception const& e ) {}
}
{
	BandMatrix<double> A(3,5,1,1);
	A.setToZero();

	A[2][1] = 2.0;

	//cout << A << endl;

	if ( A[2][1] != 2.0 ) testSuite.errorLog(name() + ": wrong element 2/1");

	try {
		A[2][0] = 3.0;
		testSuite.errorLog(name() + ": forbidden write access 2/1");
	} catch ( std::exception const& e ) {}

	try {
		A[0][2] = 3.0;
		testSuite.errorLog(name() + ": forbidden write access 2/2");
	} catch ( std::exception const& e ) {}
}
{
	BandMatrix<double> A(10,7,2,2);
	A.setToZero();
}
#endif
