#ifndef ARGUMENTINTERPRETER_H_
#define ARGUMENTINTERPRETER_H_

#include <initializer_list>
#include <map>
#include <set>
#include <string>
#include <vector>

namespace GeneHunter {

/**
 * Analyze an argument list given for a command. Three types of arguments will be distinguished:
 *
 *   command [-boolean] [-optional \<string\>] \<nonOptional\>
 *
 *   - nonOptional: must be given in a defined order, position can be in front or behind optional/boolean arguments.
 *   - optional: can be given, but when one is given it must be directly followed by a string.
 *   - boolean: can be given and is not followed by a string. It's only switching an option on/off.
 *
 * Options can be with one or two leading dashes, generally one dash are used for single char options (-h) and
 * two dashes are used for others (--version).
 */
class ArgumentInterpreter
{
public:

	enum ArgumentType { NonOptional, Optional, Boolean, NonOptionalList };

	struct ArgumentDefinition{
		std::string name_;
		ArgumentType type_;
		std::string description_;
	};

	ArgumentInterpreter( int argc, char* argv[],
		std::initializer_list<ArgumentDefinition> argumentDefinitionList = std::initializer_list<ArgumentDefinition>()
	);

	size_t getNbOfNonOptionalArguments() const {
		return nonOptionalArgumentList_.size();
	}

	std::string getNonOptionalArgument( std::string const& nonOptionalFlag ) const;

	std::vector<std::string> const& getNonOptionalList() const {
		return nonOptionalList_;
	}

	bool isOptionalFlagSet( std::string const& optionalFlag ) const;

	std::string getOptionalArgument( std::string const& optionalFlag ) const;

	bool isBooleanFlagSet( std::string const& booleanFlag ) const;

	void printUsage() const;

private:

	std::vector<std::string> nonOptionalArgumentList_;
	std::vector<std::string> nonOptionalList_;
	std::map<std::string,std::string> optionalArgumentList_;
	std::set<std::string> booleanFlagList_;

	std::vector<std::string> nonOptionalArgumentName_;
	std::vector<std::string> nonOptionalArgumentDescription_;
	std::map<std::string,std::string> optionalArgumentDescription_;
	std::map<std::string,std::string> booleanFlagDescription_;

	std::string nonOptionalListName_;
	std::string nonOptionalListDescription_;

	std::string programName_;

};

} // namespace GeneHunter

#endif /* ARGUMENTINTERPRETER_H_ */
