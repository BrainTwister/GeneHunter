#include "FullMatchManager.h"

namespace GeneAssembler {

void FullMatchManager::addMatch( std::string const& readName, Match const& match )
{
	if ( match == Match() ) return;
	auto iterFind = matches.find(readName);
	if ( iterFind == matches.end() ) matches.insert(std::make_pair(readName,match));
	else iterFind->second = match;
}

Match FullMatchManager::getMatch( std::string const& readName ) const
{
	auto iterFind = matches.find(readName);
    if ( iterFind != matches.end() ) return iterFind->second;
    else return Match();
}

} // namespace GeneAssembler
