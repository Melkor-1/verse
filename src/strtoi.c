#include "strtoi.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <ctype.h>

strtoi_errno strtoi (int *restrict out, const char *restrict nptr, 
                     int base)
{
    /*
     * Null string, empty string, leading whitespace? 
     */
    if (!nptr || !nptr[0] || isspace (nptr[0])) {
        return STRTOI_INCONVERTIBLE;
    }
    char *end_ptr;
    const int errno_original = errno;   /* We shall restore errno to its original value before returning. */
    const long int i = strtol (nptr, &end_ptr, base);

    errno = 0;

    /*
     * Both checks are needed because INT_MAX == LONG_MAX is possible.
     */
    if (i > INT_MAX || (errno == ERANGE && i == LONG_MAX)) {
        return STRTOI_OVERFLOW;
    } else if (i < INT_MIN || (errno == ERANGE && i == LONG_MIN)) {
        return STRTOI_UNDERFLOW;
    } else if (*end_ptr || nptr == end_ptr) {
        return STRTOI_INCONVERTIBLE;
    }
    *out = (int) i;
    errno = errno_original;
    return STRTOI_SUCCESS;
}
