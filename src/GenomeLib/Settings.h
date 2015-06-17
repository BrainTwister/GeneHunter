#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <iostream>

namespace GeneHunter {

enum SearchAlgorithm {
	FirstTokensAsSeed,
	TakeMatchWithMostSeeds
};

std::ostream& operator << ( std::ostream& os, SearchAlgorithm s );
std::istream& operator >> ( std::istream& is, SearchAlgorithm& s );

} // namespace GeneHunter

#endif /* SETTINGS_H_ */
