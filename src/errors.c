#include "errors.h"

#include <assert.h>

const char *get_err_msg(int err_code)
{
    static_assert(ARRAY_CARDINALITY(errors) - 1 == E_CURL_PERFORM_FAILED,
                  "The errors array and the enum must be kept in-sync!");

    if (err_code >= 0 && err_code < (int) ARRAY_CARDINALITY(errors)) {
        return errors[err_code];
    }
    return "Unknown error code.\n";
}
