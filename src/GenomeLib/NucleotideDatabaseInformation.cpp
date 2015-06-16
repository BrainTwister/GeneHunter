#include "NucleotideDatabaseInformation.h"

using namespace std;

namespace GeneHunter {

ostream& operator << ( ostream& os, NucleotideDatabaseInformation const& entry )
{
	return os << " Number of imported gene sequences         = " << entry.nbImportedGeneSeq_ << endl
	          << " Min length of imported gene sequences     = " << entry.minGeneSeqSize_ << endl
	          << " Max length of imported gene sequences     = " << entry.maxGeneSeqSize_ << endl
	          << " Total number of bases                     = " << entry.totalNbOfBases_ << endl
	          << " Average length of imported gene sequences = " << static_cast<double>(entry.totalNbOfBases_) / entry.nbImportedGeneSeq_ << endl;
}

} // namespace GeneHunter
