#ifndef TOKENITERATOR_H_
#define TOKENITERATOR_H_

#include "GeneHunterException.h"
#include "Sequence.h"
#include "SequenceToken.h"
#include <boost/iterator/iterator_facade.hpp>

namespace GeneHunter {

/**
 * N: TokenSize
 * M: StepSize
 * Constructor is only allowed if the StepSize is a multiple of the compressionFactor of the input sequence type.
 * For StepSize=1 input sequence type have to be char.
 * For StepSize%2 == 0 CompressedCharTwoInOne is possible.
 * CompileTime check: enable_if< StepSize % GetNbBaseItemsInChar<T> == 0 >
 */
template < class OutTokenType, size_t TokenSize, class InSeqType = OutTokenType, size_t StepSize = TokenSize >
class TokenIterator
 : public boost::iterator_facade <
   	   TokenIterator<OutTokenType,TokenSize,InSeqType,StepSize>,
   	   std::pair< SequenceToken<OutTokenType,TokenSize>, size_t > const,
   	   boost::forward_traversal_tag
   >
{
public:

	typedef TokenIterator<OutTokenType,TokenSize,InSeqType,StepSize> Self;
	typedef std::pair< SequenceToken<OutTokenType,TokenSize>, size_t > Value;

	// End position
	TokenIterator()
	 : iterTokenBeg_(),
	   iterSequenceEnd_(),
	   currentValue_(),
	   position_(0)
	{}

	explicit TokenIterator( Sequence<InSeqType> const& sequence )
	 : iterTokenBeg_(sequence.begin()),
	   iterSequenceEnd_(sequence.end()),
	   currentValue_(),
	   position_(0)
	{
		increment();
	}

private:

	typedef typename Sequence<InSeqType>::ConstIterator SequenceConstIterator;
	static const size_t compressionFaktorInSeq = GetNbBaseItemsInChar<InSeqType>::value;

    friend class boost::iterator_core_access;

    Value const& dereference() const
    {
        return currentValue_;
    }

    bool equal( Self const& other ) const
    {
        return iterTokenBeg_ == other.iterTokenBeg_;
    }

    void increment()
    {
		int nbValidBases(0);
		size_t distance(0);
		SequenceConstIterator iterTokenCur(iterTokenBeg_);

    	while ( iterTokenCur != iterSequenceEnd_ )
    	{
    		if ( ! isValid(*iterTokenCur) ) {
    			nbValidBases = 0;
        		++iterTokenCur;
        		distance += compressionFaktorInSeq;
    			iterTokenBeg_ = iterTokenCur;
    			position_ += distance;
    			distance = 0;
    		} else {
        		++iterTokenCur;
        		nbValidBases += compressionFaktorInSeq;
        		distance += compressionFaktorInSeq;
    		}

    		if ( nbValidBases == TokenSize ) {
    			break;
    		}
    	}

    	if ( nbValidBases == TokenSize ) {

    		SequenceConstIterator iterTokenEnd = iterTokenBeg_ + distance / compressionFaktorInSeq;

			try {
			    currentValue_ = Value( std::make_pair(
			    	SequenceToken<OutTokenType,TokenSize>(iterTokenBeg_,iterTokenEnd), position_)
			    );
    	    } catch ( GeneHunterException const& e ) {
    			std::cout << "position = " << position_ << std::endl;
    			std::cout << "dist = " << std::distance(iterTokenBeg_,iterTokenEnd) << std::endl;
    			std::cout << "iterTokenBeg = " << *iterTokenBeg_ << std::endl;
    			std::cout << "iterTokenEnd = " << *iterTokenEnd << std::endl;
    			throw;
    	    }

    	    if ( std::distance(iterTokenBeg_,iterSequenceEnd_) <= TokenSize / compressionFaktorInSeq ) {
        	    iterTokenBeg_ = iterSequenceEnd_;
        	    position_ = 0;
    	    } else {
				iterTokenBeg_ += StepSize / compressionFaktorInSeq;
				position_ += StepSize;
    	    }

    	} else {

    		iterTokenBeg_ = SequenceConstIterator();

    	}
    }

    bool isValid( InSeqType const& t ) const {
    	return containOnlyPureBases(t) or containChar(t,'X');
    }

	SequenceConstIterator iterTokenBeg_;

	SequenceConstIterator iterSequenceEnd_;

	Value currentValue_;

	size_t position_;

};

} // namespace GeneHunter

#endif /* TOKENITERATOR_H_ */
