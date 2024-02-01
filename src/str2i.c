#include "str2i.h"

#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <ctype.h>

str2i_errno str2i(int *restrict out, const char *restrict nptr, int base)
{
    if (!out) {
        return STR2I_INCONVERTIBLE;
    }

    if (!nptr || isspace((unsigned char)nptr[0])) {
        *out = 0;
        return STR2I_INCONVERTIBLE;
    }
    
    if (!(base == 0 || (base >= 2 && base <= 36))) {
        *out = 0;
        return STR2I_INCONVERTIBLE;
    }

    char *end_ptr;
    int errno_original = errno;   /* We shall restore errno to its original value before returning. */
    const long int val = strtol(nptr, &end_ptr, base);
    int errno_sample = errno;
    errno = errno_original;


    /*
     * Both checks are needed because INT_MAX == LONG_MAX is possible.
     */
    if (val > INT_MAX || (errno_sample == ERANGE && val == LONG_MAX)) {
        *out = INT_MAX;
        return STR2I_OVERFLOW;
    } 

    if (val < INT_MIN || (errno == ERANGE && val == LONG_MIN)) {
        *out = INT_MIN;
        return STR2I_UNDERFLOW;
    } 

    *out = (int) val;
    return STR2I_SUCCESS;
}
