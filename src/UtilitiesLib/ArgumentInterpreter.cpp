#include "ArgumentInterpreter.h"
#include <boost/lexical_cast.hpp>
#include <stdexcept>
#include <iomanip>

using namespace std;

namespace GeneAssembler {

ArgumentInterpreter::ArgumentInterpreter( int argc, char* argv[],
    initializer_list<ArgumentDefinition> argumentDefinitionList
)
 : programName_(argv[0])
{
	// Interpret initializer_list
	for ( auto const& elem : argumentDefinitionList ) {
		if ( elem.type_ == NonOptional ) {
			nonOptionalArgumentName_.push_back(elem.name_);
			nonOptionalArgumentDescription_.push_back(elem.description_);
		} else if ( elem.type_ == Optional ) {
			optionalArgumentDescription_.insert(make_pair(elem.name_,elem.description_));
		} else if ( elem.type_ == Boolean ) {
			booleanFlagDescription_.insert(make_pair(elem.name_,elem.description_));
		} else if ( elem.type_ == NonOptionalList ) {
			if (!nonOptionalListName_.empty()) throw runtime_error("ArgumentInterpreter: Only one NonOptionalList is allowed.");
			nonOptionalListName_ = elem.name_;
			nonOptionalListDescription_ = elem.description_;
		} else
			throw runtime_error("ArgumentInterpreter: ArgumentType not found.");
	}

	// Starting with index 1, because first entry is program name
    for ( int i(1); i != argc; ++i ) {
    	string currentString = argv[i];
    	if ( currentString[0] == '-' ) {

    		// remove first dash
    		currentString.erase(0,1);

    		// remove a possible second dash
    		if ( currentString[0] == '-' ) currentString.erase(0,1);

    		if ( currentString == "help" ) {
    			printUsage();
    			exit(0);
    		}

    		if ( optionalArgumentDescription_.find(currentString) != optionalArgumentDescription_.end() ) {
    			string errorMessage("ArgumentInterpreter: no argument found for option \"-" + currentString + "\".");
    			if ( ++i >= argc ) throw runtime_error(errorMessage);
    			string nextString = argv[i];
    			if ( nextString[0] == '-' ) throw runtime_error(errorMessage);
    			optionalArgumentList_.insert(make_pair(currentString,nextString));
    		} else if ( booleanFlagDescription_.find(currentString) != booleanFlagDescription_.end() ) {
    			booleanFlagList_.insert(currentString);
    		} else {
    			printUsage();
    			throw runtime_error("ArgumentInterpreter: \"-" + currentString + "\" is not a valid option.");
    		}
    	} else {
    		if ( nonOptionalArgumentList_.size() < nonOptionalArgumentName_.size() ) {
    		    nonOptionalArgumentList_.push_back(currentString);
    		} else {
    		    nonOptionalList_.push_back(currentString);
    		}
    	}
    }

	if ((nonOptionalArgumentList_.size() < nonOptionalArgumentName_.size()) or
	    (!nonOptionalListName_.empty() and nonOptionalList_.size() == 0))
	{
	    printUsage();
	    throw runtime_error("Wrong number of arguments.");
	}
}

string ArgumentInterpreter::getNonOptionalArgument( string const& nonOptionalFlag ) const
{
	vector<string>::const_iterator iterFind = find(nonOptionalArgumentName_.begin(),nonOptionalArgumentName_.end(),nonOptionalFlag);
    if ( iterFind == nonOptionalArgumentList_.end() )
    	throw runtime_error("ArgumentInterpreter: \"" + nonOptionalFlag + "\" is not valid.");
    return nonOptionalArgumentList_[distance(nonOptionalArgumentName_.begin(),iterFind)];
}

string ArgumentInterpreter::getOptionalArgument( string const& optionalFlag ) const
{
	std::map<std::string,std::string>::const_iterator iterFind = optionalArgumentList_.find(optionalFlag);
    if ( iterFind == optionalArgumentList_.end() )
    	throw runtime_error("ArgumentInterpreter: \"" + optionalFlag + "\" is not a valid option.");
    return iterFind->second;
}

bool ArgumentInterpreter::isOptionalFlagSet( string const& optionalFlag ) const
{
    return optionalArgumentList_.find(optionalFlag) != optionalArgumentList_.end();
}

bool ArgumentInterpreter::isBooleanFlagSet( string const& booleanFlag ) const
{
    return booleanFlagList_.find(booleanFlag) != booleanFlagList_.end();
}

void ArgumentInterpreter::printUsage() const
{
	size_t maxWidth(20);
	for ( auto const& elem : nonOptionalArgumentName_ ) if ( elem.size() > maxWidth ) maxWidth = elem.size();
	for ( auto const& elem : optionalArgumentDescription_ ) if ( elem.first.size() > maxWidth ) maxWidth = elem.first.size();
	for ( auto const& elem : booleanFlagDescription_ ) if ( elem.first.size() > maxWidth ) maxWidth = elem.first.size();
	maxWidth += 2;

    cout << "Usage: " << programName_ << " ";
    for ( auto const& name : nonOptionalArgumentName_ ) cout << "<" << name << "> ";
    if ( !nonOptionalListName_.empty() ) cout << "<" << nonOptionalListName_ << "> ";

    cout << "\n\nNon optional: " << endl;
    for ( uint8_t i(0); i != nonOptionalArgumentName_.size(); ++i )
    	cout << "    " << left << setw(maxWidth) << nonOptionalArgumentName_[i] << nonOptionalArgumentDescription_[i] << endl;
    if (!nonOptionalListName_.empty()) cout << "    " << left << setw(maxWidth) << nonOptionalListName_ << nonOptionalListDescription_ << endl;

    cout << "\nOptional: " << endl;
    cout << "    " << left << setw(maxWidth) << "help" << "Printing this output." << endl;
    for ( auto const& elem : optionalArgumentDescription_ ) cout << "    " << left << setw(maxWidth) << elem.first << elem.second << endl;
    for ( auto const& elem : booleanFlagDescription_ ) cout << "    " << left << setw(maxWidth) << elem.first << elem.second << endl;
    cout << endl;
}

} // namespace GeneAssembler
