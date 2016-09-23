#include "BrainTwister/ArgumentParser.h"
#include "gtest/gtest.h"

namespace bt = BrainTwister;

TEST(ArgumentParserTest, Test1)
{
    char *argv[] = {strdup("program"), strdup("--input"), strdup("file"), strdup("arg1"), strdup("arg2")};
    int argc = (int)(sizeof(argv) / sizeof(argv[0]));

    bt::ArgumentParser arg(argc, argv, "1.0",
        {{"file1", bt::Value<std::string>(), "File 1."},
         {"file2", bt::Value<std::string>(), "File 2."}},
        {{"input", "i", bt::Value<std::string>(), "Description of the option input."},
         {"verbose", "v", bt::Value<std::string>("foo"), "Print more output."}}
    );

    EXPECT_EQ("arg1", arg.get<std::string>("file1"));
    EXPECT_EQ("arg2", arg.get<std::string>("file2"));
    EXPECT_EQ("file", arg.get<std::string>("input"));
    EXPECT_EQ("foo", arg.get<std::string>("verbose"));
}

TEST(ArgumentParserTest, Test2)
{
    char *argv[] = {strdup("program"), strdup("7")};
    int argc = (int)(sizeof(argv) / sizeof(argv[0]));

    bt::ArgumentParser arg(argc, argv, "1.0",
        {{"integer", bt::Value<int>(), "Required integer."}}
    );

    EXPECT_EQ(7, arg.get<int>("integer"));
}

TEST(ArgumentParserTest, Test3)
{
    char *argv[] = {strdup("program")};
    int argc = (int)(sizeof(argv) / sizeof(argv[0]));

    EXPECT_THROW(
		bt::ArgumentParser arg(argc, argv, "1.0",
			{{"integer", bt::Value<int>(), "Required integer."}}
		);
    , ArgumentParserException);
}

TEST(ArgumentParserTest, Test4)
{
    char *argv[] = {strdup("program"), strdup("1"), strdup("2")};
    int argc = (int)(sizeof(argv) / sizeof(argv[0]));

    bt::ArgumentParser arg(argc, argv, "1.0",
		{{"integer", bt::Value<std::vector<int>>(), "List of integers."}}
	);

    EXPECT_EQ(1, arg.get<std::vector<int>>("integer")[0]);
}
