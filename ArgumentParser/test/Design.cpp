#include <memory>
#include <string>
#include <vector>
#include "gtest/gtest.h"

struct ValueBase
{
    virtual ~ValueBase() {}
};

template <class T>
struct Value : public ValueBase
{
    Value(T t) : defaultValue(t) {}

    T defaultValue;
};

struct ValueList
{
    template <class T1, class T2>
    ValueList(T1 const& t1, T2 const& t2) : values{std::shared_ptr<T1>(new T1(t1)), std::shared_ptr<T2>(new T2(t2))} {}

    template <class T>
    T get(size_t pos) const {
        return std::static_pointer_cast< Value<T> >(values[pos])->defaultValue;
    }

    std::vector< std::shared_ptr<ValueBase> > values;
};

TEST(DesignTest, static_pointer_cast)
{
    std::shared_ptr<ValueBase> ptrValue(new Value<int>(5));

    EXPECT_EQ(5, std::static_pointer_cast< Value<int> >(ptrValue)->defaultValue);
}

TEST(DesignTest, ValueList)
{
    ValueList valueList(Value<int>(3), Value<std::string>("foo"));

    EXPECT_EQ(3, valueList.get<int>(0));
    EXPECT_EQ("foo", valueList.get<std::string>(1));
}

