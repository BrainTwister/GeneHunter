#include "Location.h"
#include "gtest/gtest.h"

using namespace std;
using namespace GeneHunter;

TEST(SequenceTest, Default)
{
    Location location;

    EXPECT_FALSE(location.match(29, 30);

    if (location.getEnclosingRange() != Range<uint32_t>())
        testSuite.errorLog(name() + ": error 1/2");
}

TEST(SequenceTest, Join2)
{
    Location location("join(complement(4918..5163),complement(2691..4571))");

    if (!location.match(4920, 5097)) testSuite.errorLog(name() + ": error 2/1");
    if (!location.match(4910, 5097)) testSuite.errorLog(name() + ": error 2/2");
    if (!location.match(2877, 4539)) testSuite.errorLog(name() + ": error 2/3");

    if (!location.within(4920, 5097))
        testSuite.errorLog(name() + ": error 2/4");
    if (location.within(4910, 5097)) testSuite.errorLog(name() + ": error 2/5");
    if (!location.within(2877, 4539))
        testSuite.errorLog(name() + ": error 2/6");

    if (location.getEnclosingRange() != Range<uint32_t>(2691, 5163))
        testSuite.errorLog(name() + ": error 2/7");
}

TEST(SequenceTest, Join2)
{
    Location location("join(complement(<4918..5163),complement(2691..>4571))");

    if (!location.match(4910, 5097)) testSuite.errorLog(name() + ": error 3/1");
    if (!location.match(4910, 5164)) testSuite.errorLog(name() + ": error 3/2");

    if (!location.within(4910, 5097))
        testSuite.errorLog(name() + ": error 3/3");
    if (location.within(4910, 5164)) testSuite.errorLog(name() + ": error 3/4");
}

TEST(SequenceTest, Single)
{
    Location location("6225");
    if (!location.match(6225, 6225)) testSuite.errorLog(name() + ": error 4/1");

    if (location.getEnclosingRange() != Range<uint32_t>(6225, 6225))
        testSuite.errorLog(name() + ": error 4/2");
}

