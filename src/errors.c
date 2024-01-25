#include "errors.h"

#include <assert.h>

/* 
 * Array of strings to map enum error types to printable string. 
 */
static const char *const errors[] = {
	/* *INDENT-OFF* */
	[E_NULL_ARGV] =
    	"Error: A NULL argv[0] was passed through an exec system call.\n",
	[E_INSUFFICIENT_ARGS] =
    	"Usage: verse <chapter> <verse>\n",
	[E_INVALID_CHAPTER] = 
  		"Error: Invalid chapter number.\n",
	[E_INVALID_VERSE] = 
		"Error: Invalid verse number for the given chapter.\n",
	[E_INVALID_RANGE] = 
		"Error: Chapter or verse out of valid numeric range.\n",
	[E_PARSE_ERROR] = 
		"Error: Non-numeric input for chapter or verse.\n",
	[E_ENOMEM] = 
  		"Error: Insufficient memory.\n",
	[E_UNKNOWN] = 
  		"Fatal: An unknown error has arisen.\n",
    [E_CURL_INIT_FAILED] = 
		"Error: curl_easy_init() failed.\n",
	[E_CURL_PERFORM_FAILED] = 
		"Error: curl_easy_perform() failed.\n"
	/* *INDENT-ON* */
};

const char *get_err_msg(int err_code)
{
    static_assert(ARRAY_CARDINALITY(errors) - 1 == E_CURL_PERFORM_FAILED,
                  "The errors array and the enum must be kept in-sync!");

    if (err_code >= 0 && err_code < (int) ARRAY_CARDINALITY(errors)) {
        return errors[err_code];
    }
    return "Unknown error code.\n";
}
