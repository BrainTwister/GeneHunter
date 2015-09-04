// Copyright (C) 2015, Bernd Doser (service@braintwister.eu)
// All rights reserved.
//
// This file is part of ArgumentParser
//
// ANY USE OF THIS CODE CONSTITUTES ACCEPTANCE OF THE
// TERMS OF THE COPYRIGHT NOTICE

#ifndef VALUE_H_
#define VALUE_H_

#include <memory>
#include <sstream>
#include <string>

namespace BrainTwister {

struct ValueBase
{
    virtual ~ValueBase() {};

    virtual void setValue(std::string const& str) = 0;
};

template <class T>
struct TypedValue : public ValueBase
{
    TypedValue() : value() {}

    TypedValue(T const& value) : value(value) {}

    virtual void setValue(std::string const& str) {
        std::stringstream ss;
        ss << str;
        ss >> value;
    }

    T value;
};

/// Primary function called by ArgumentDescription
template <class T>
std::shared_ptr<ValueBase> Value()
{
    return std::make_shared< TypedValue<T> >();
}

/// Primary function called by ArgumentDescription
template <class T>
std::shared_ptr<ValueBase> Value(T const& value)
{
    return std::make_shared< TypedValue<T> >(value);
}

} // namespace BrainTwister

#endif /* VALUE_H_ */

