// Copyright (C) 2015, Bernd Doser (service@braintwister.eu)
// All rights reserved.
//
// This file is part of ArgumentParser
//
// ANY USE OF THIS CODE CONSTITUTES ACCEPTANCE OF THE
// TERMS OF THE COPYRIGHT NOTICE

#ifndef ARGUMENTPARSER_H_
#define ARGUMENTPARSER_H_

#include "BrainTwister/ArgumentParserException.h"
#include <BrainTwister/Value.h>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace BrainTwister {

/// Definition of an required argument
struct RequiredArgumentDefinition
{
    RequiredArgumentDefinition(std::string const& label, std::shared_ptr<ValueBase> value, std::string const& description)
     : label(label), value(value), description(description)
    {}

    std::string label;
    std::shared_ptr<ValueBase> value;
    std::string description;
};

/// Definition of an optional argument
struct OptionalArgumentDefinition
{
    OptionalArgumentDefinition(std::string const& label, std::string const& shortLabel,
        std::shared_ptr<ValueBase> value, std::string const& description)
     : label(label), shortLabel(shortLabel), value(value), description(description)
    {}

    std::string label;
    std::string shortLabel;
    std::shared_ptr<ValueBase> value;
    std::string description;
};

/**
 * Parse program arguments.
 */
class ArgumentParser
{
public:

    ArgumentParser(int argc, char* argv[], std::string const& version,
        std::vector<RequiredArgumentDefinition> const& reqArgDefs = std::vector<RequiredArgumentDefinition>(),
        std::vector<OptionalArgumentDefinition> const& optArgDefs = std::vector<OptionalArgumentDefinition>());

    template <class T>
    T const& get(std::string const& label) const
    {
        for (auto & def : reqArgDefs) if (def.label == label) return std::static_pointer_cast< TypedValue<T> >(def.value)->value;
        for (auto & def : optArgDefs) if (def.label == label) return std::static_pointer_cast< TypedValue<T> >(def.value)->value;
        throw std::runtime_error("Option " + label + " does not exist.");
    }

    void printUsage() const;

private:

    // Program name from argv[0].
    std::string programName;

    // Verison string.
    std::string version;

    // List of required argument definitions.
    std::vector<RequiredArgumentDefinition> reqArgDefs;

    // List of optional argument definitions.
    std::vector<OptionalArgumentDefinition> optArgDefs;

};

} // namespace BrainTwister

#endif /* ARGUMENTPARSER_H_ */
