#include "BrainTwister/ArgumentParser.h"
#include <iomanip>
#include <iostream>

namespace BrainTwister {

ArgumentParser::ArgumentParser(int argc, char* argv[], std::string const& version,
    std::vector<RequiredArgumentDefinition> const& reqArgDefs,
    std::vector<OptionalArgumentDefinition> const& optArgDefs)
 : programName(argv[0]),
   version(version),
   reqArgDefs(reqArgDefs),
   optArgDefs(optArgDefs)
{
    programName = programName.substr(programName.find_last_of("/") + 1);

    this->optArgDefs.push_back(OptionalArgumentDefinition("help", "h", Value<bool>(false), "Print help page."));
    this->optArgDefs.push_back(OptionalArgumentDefinition("version", "v", Value<std::string>(), "Print version string."));

    // Starting with index 1, because first entry is the name of the program
    size_t nbRegArgs = 0;
    for (int i = 1; i != argc; ++i)
    {
        std::string current = argv[i];
        if (current.compare(0, 2, "--") == 0)
        {
            // remove dashs
            current.erase(0,2);

            if (current == "help") {
                printUsage();
                exit(0);
            }

            if (current == "version") {
                std::cout << version << std::endl;
                exit(0);
            }

            bool found = false;
            for (auto & option : optArgDefs)
            {
                if (option.label != current) continue;
                std::string errorMessage("ArgumentParser: no argument found for option \"-" + current + "\".");
                if ( ++i >= argc ) throw std::runtime_error(errorMessage);
                std::string nextString = argv[i];
                if ( nextString[0] == '-' ) throw std::runtime_error(errorMessage);
                option.value->setValue(nextString);
                found = true;
            }

            if (!found) {
                printUsage();
                throw std::runtime_error("ArgumentParser: \"--" + current + "\" is not a valid option.");
            }
        } else if (current.compare(0, 1, "-") == 0) {

            // remove dash
            current.erase(0,1);

            if (current == "h") {
                printUsage();
                exit(0);
            }

            if (current == "v") {
                std::cout << version << std::endl;
                exit(0);
            }

            if (current.size() != 1) throw std::runtime_error("Short label has more than one letter.");

            bool found = false;
            for (auto & option : optArgDefs)
            {
                if (option.shortLabel != current) continue;
                std::string errorMessage("ArgumentParser: no argument found for option \"-" + current + "\".");
                if ( ++i >= argc ) throw std::runtime_error(errorMessage);
                std::string nextString = argv[i];
                if ( nextString[0] == '-' ) throw std::runtime_error(errorMessage);
                option.value->setValue(nextString);
                found = true;
            }

            if (!found) {
                printUsage();
                throw std::runtime_error("ArgumentParser: \"-" + current + "\" is not a valid option.");
            }

        } else {
            reqArgDefs[nbRegArgs++].value->setValue(current);
        }
    }

    if (nbRegArgs != reqArgDefs.size()) {
        printUsage();
        throw ArgumentParserException("Wrong number of required arguments.");
    }
}

void ArgumentParser::printUsage() const
{
    // Determnie maximal size of options
    size_t maxWidth = 20;
    for (auto const& elem : reqArgDefs) if ( elem.label.size() > maxWidth ) maxWidth = elem.label.size();
    for (auto const& elem : optArgDefs) if ( elem.label.size() > maxWidth ) maxWidth = elem.label.size();
    maxWidth += 2;

    // Print usage string
    std::cout << "Usage: " << programName;
    for (auto const& elem : reqArgDefs) std::cout << " <" << elem.label << ">";
    for (auto const& elem : optArgDefs) std::cout << " [--" << elem.label << ", -" << elem.shortLabel << "]";
    std::cout << "\n" << std::endl;

    // Print required arguments
    std::cout << "Required arguments: " << std::endl;
    for (auto const& elem : reqArgDefs) {
        std::cout << "    " << std::left << std::setw(maxWidth) << elem.label << elem.description << std::endl;
    }
    std::cout << std::endl;

    // Print optional arguments
    std::cout << "Optional arguments: " << std::endl;
    for (auto const& elem : optArgDefs) {
        std::string optionString("--" + elem.label + ", -" + elem.shortLabel);
        std::cout << "    " << std::left << std::setw(maxWidth) << optionString << elem.description << std::endl;
    }
    std::cout << std::endl;
}

} // namespace BrainTwister

