#ifndef FATALERROR_H_
#define FATALERROR_H_

#include <cassert>
#include <stdexcept>

#if defined GENEASSEMBLER_ASSERT_FATAL_ERROR
    #define GENEASSEMBLER_FATAL_ERROR(check) assert(check)
#elif defined GENEASSEMBLER_THROW_FATAL_ERROR
    #define GENEASSEMBLER_FATAL_ERROR(check) if (check) throw std::runtime_error("Fatal error: ");
#else
    #define GENEASSEMBLER_FATAL_ERROR(check)
#endif

#endif /* FATALERROR_H_ */
