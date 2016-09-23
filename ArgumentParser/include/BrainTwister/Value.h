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
#include <vector>

namespace BrainTwister {

struct ValueBase
{
    virtual ~ValueBase() {};

    virtual void setValue(std::string const& str) = 0;

    virtual bool isVector() const = 0;
};

/// Primary class
template <class T>
struct Value : public ValueBase
{
	Value(T const& value = T()) : value(value) {}

    virtual void setValue(std::string const& str) {
        std::stringstream ss;
        ss << str;
        ss >> value;
    }

    virtual bool isVector() const { return false; }

	T value;
};

/// Specialization for std::vector
template <class T>
struct Value<std::vector<T>> : public ValueBase
{
	Value(std::vector<T> const& value = std::vector<T>()) : value(value) {}

    virtual void setValue(std::string const& str) {
        std::stringstream ss;
        ss << str;
        T tmp;
        ss >> tmp;
        value.push_back(tmp);
    }

    virtual bool isVector() const { return true; }

	std::vector<T> value;
};

} // namespace BrainTwister

#endif /* VALUE_H_ */

