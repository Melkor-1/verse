#ifndef ERRORS_H
#define ERRORS_H

#include <stddef.h>

#define ARRAY_CARDINALITY(x) (sizeof (x) / sizeof ((x)[0]))

/*
 *	Error codes for invalid arguments.
 */
enum error_codes {
    E_SUCCESS = 0,
    E_NULL_ARGV,
    E_INSUFFICIENT_ARGS,
    E_INVALID_CHAPTER,
    E_INVALID_VERSE,
    E_INVALID_RANGE,
    E_PARSE_ERROR,
    E_ENOMEM,
    E_UNKNOWN,
    E_CURL_INIT_FAILED,
    E_CURL_PERFORM_FAILED
};

/**
*   @brief      get_err_msg() shall retrieve the error message corresponding to the given error code. 
*  
*   @param      err_code - An integer respresenting the error code.
*  
*   @return     A pointer to a constant string containing the error message.
*               If the error code is not recognized, a default "Unknown error code.\n" 
*               message is returned.
*/
const char *get_err_msg(int err_code);

#endif                          /* ERRORS_H */
