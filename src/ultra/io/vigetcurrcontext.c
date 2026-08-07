#include "PR/os_internal.h"
#include "PRinternal/viint.h"

#ident "$Revision: 1.17 $"

__OSViContext *__osViGetCurrentContext(void) {
    return __osViCurr;
}
