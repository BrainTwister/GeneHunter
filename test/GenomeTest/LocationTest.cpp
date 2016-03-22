#include "GenomeLib/Location.h"
#include <gtest/gtest.h>

using namespace GeneHunter;

TEST(SequenceTest, Default)
{
    Location location;

    EXPECT_FALSE(location.match(29, 30));
    EXPECT_EQ(Range<uint32_t>(), location.getEnclosingRange());
}

TEST(SequenceTest, Join)
{
    Location location("join(complement(4918..5163),complement(2691..4571))");

    EXPECT_TRUE(location.match(4920, 5097));
    EXPECT_TRUE(location.match(4910, 5097));
    EXPECT_TRUE(location.match(2877, 4539));
    EXPECT_TRUE(location.within(4920, 5097));
    EXPECT_FALSE(location.within(4910, 5097));
    EXPECT_TRUE(location.within(2877, 4539));
    EXPECT_EQ(Range<uint32_t>(2691, 5163), location.getEnclosingRange());
}

TEST(SequenceTest, Join2)
{
    Location location("join(complement(<4918..5163),complement(2691..>4571))");

    EXPECT_TRUE(location.match(4910, 5097));
    EXPECT_TRUE(location.match(4910, 5164));
    EXPECT_TRUE(location.within(4910, 5097));
    EXPECT_FALSE(location.within(4910, 5164));
}

TEST(SequenceTest, Single)
{
    Location location("6225");

    EXPECT_TRUE(location.match(6225, 6225));
    EXPECT_EQ(Range<uint32_t>(6225, 6225), location.getEnclosingRange());
}
