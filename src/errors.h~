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

/**
*   @brief      get_err_msg() shall retrieve the error message corresponding to the given error code. 
*  
*   @param      err_code - An integer respresenting the error code.
*  
*   @return     A pointer to a constant string containing the error message.
*               If the error code is not recognized, a default "Unknown error code.\n" 
*               message is returned.
*/
const char *get_err_msg (int err_code);

#endif   /* ERRORS_H */
