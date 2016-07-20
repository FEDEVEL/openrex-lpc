#ifndef __ASSERT_H__
#define __ASSERT_H__

/* NOTE: if you want to use BOARD assertion in translation unit (ergo some_file.c), 
 * '#include "assert.h"' must be the very first include */

/* expected prototype */
void BOARD_assert(const char *file, const int line);

/* if other assert is not in use and NDEBUG
 * is not set, implement our assertion */
#ifndef assert
#   ifndef NDEBUG
#       define assert(condition) ((!(condition)) ? BOARD_assert(__FILE__, __LINE__) : 0)
#   else
#       define assert(condition)
#   endif
#endif

#endif
