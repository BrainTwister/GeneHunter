#ifndef FATALERROR_H_
#define FATALERROR_H_

#include <cassert>
#include <stdexcept>

#if defined GENEHUNTER_ASSERT_FATAL_ERROR
    #define GENEHUNTER_FATAL_ERROR(check) assert(check)
#elif defined GENEHUNTER_THROW_FATAL_ERROR
    #define GENEHUNTER_FATAL_ERROR(check) if (check) throw std::runtime_error("Fatal error: ");
#else
    #define GENEHUNTER_FATAL_ERROR(check)
#endif

#endif /* FATALERROR_H_ */
