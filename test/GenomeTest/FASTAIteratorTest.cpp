#include "GenomeLib/FASTAIterator.h"
#include <gtest/gtest.h>

using namespace GeneHunter;

TEST(FASTAIteratorTest, default_constructor)
{
	FASTAIterator<char> iterCur;
}
