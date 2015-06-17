#ifndef FASTA_H_
#define FASTA_H_

#include "Genome.h"
#include "Sequence.h"
#include <boost/algorithm/string/trim.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/tokenizer.hpp>
#include <iostream>
#include <string>
#include <functional>

namespace GeneHunter {

template < class T >
class FASTA
{
public:

	FASTA() : nuclGI_(0) {}

	FASTA( NuclGIType geneID, std::string const& sequence )
	 : nuclGI_(geneID), sequence_(sequence)
	{}

	void setGeneID( size_t geneID ) { nuclGI_ = geneID; }
	void setSequence( std::string const& sequence ) { sequence_ = Sequence<T>(sequence); }

	NuclGIType getGeneID() const { return nuclGI_; }
	Sequence<T> const& getSequence() const { return sequence_; }

	static NuclGIType getFirstGeneID( std::string const& header );

private:

    friend class boost::serialization::access;

    template < class Archive >
    void serialize( Archive & ar, const unsigned int )
    {
        ar & nuclGI_;
        ar & sequence_;
    }

    NuclGIType nuclGI_;

    Sequence<T> sequence_;

};

template < class T >
NuclGIType FASTA<T>::getFirstGeneID( std::string const& header )
{
	if ( header[0] != '>' )
		throw GeneHunterException("FASTA<T>::getFirstGeneID: Header doesn't start with >.");

	typedef boost::tokenizer<boost::char_separator<char> > tokenizer;

	// hexadecimal code for ctrl-A "\001" to separate redundant entries
	boost::char_separator<char> sep1("\001");
	tokenizer tokens1(header,sep1);
	tokenizer::const_iterator iterCur1 = tokens1.begin();

	boost::char_separator<char> sep2("|");
	tokenizer tokens2(*iterCur1,sep2);
	tokenizer::const_iterator iterCur = tokens2.begin();
	++iterCur;
	return boost::lexical_cast<NuclGIType>(boost::algorithm::trim_copy(*iterCur));

#if 0
	++iterCur;
	ptrFASTA_->setDatabaseName(boost::algorithm::trim_copy(*iterCur)); ++iterCur;
	ptrFASTA_->setDatabaseID(boost::algorithm::trim_copy(*iterCur)); ++iterCur;
	ptrFASTA_->setDescription(boost::algorithm::trim_copy(*iterCur)); ++iterCur;

	if ( iterCur != tokens2.end() )
		throw GeneHunterException("FASTAIterator::increment: Number of tokens greater 5 in header. (line = " + line + ")");
#endif
}

template < class T >
using PtrFASTA = boost::shared_ptr< FASTA<T> >;

template < class T >
inline std::ostream& operator << ( std::ostream& os, FASTA<T> const& entry )
{
	return os << "geneID       = " << entry.getGeneID() << std::endl
	          << "sequence     = " << entry.getSequence() << std::endl;
}

template < class T >
inline std::ostream& operator << ( std::ostream& os, PtrFASTA<T> const& p )
{
	return os << *p << std::endl;
}

template < class T >
inline bool operator < ( PtrFASTA<T> const& p1, PtrFASTA<T> const& p2 )
{
	return p1->getGeneID() < p2->getGeneID();
}

} // namespace GeneHunter

namespace std {

template < class T >
struct hash< GeneHunter::PtrFASTA<T> >
{
	size_t operator () ( GeneHunter::PtrFASTA<T> const& p ) const
	{
		return hash<GeneHunter::NuclGIType>()(p->getGeneID());
	}
};

template < class T >
struct equal_to< GeneHunter::PtrFASTA<T> >
{
	bool operator () ( GeneHunter::PtrFASTA<T> const& p1, GeneHunter::PtrFASTA<T> const& p2 ) const
	{
		return p1->getGeneID() == p2->getGeneID();
	}
};

} // namespace std

#endif /* FASTA_H_ */
