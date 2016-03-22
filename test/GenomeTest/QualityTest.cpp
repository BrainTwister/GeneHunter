#include "GenomeLib/Quality.h"
#include <gtest/gtest.h>

using namespace GeneHunter;

TEST(QualityTest, Default)
{
    EXPECT_NEAR(1.0, static_cast<double>(FastqSanger::data[33]), 1e-4);
    EXPECT_NEAR(5.01187e-10, static_cast<double>(FastqSanger::data[126]), 1e-4);
}
