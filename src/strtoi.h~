#ifndef STRTOI_H
#define STRTOI_H

typedef enum {
    STRTOI_SUCCESS,
    STRTOI_OVERFLOW,
    STRTOI_UNDERFLOW,
    STRTOI_INCONVERTIBLE
} strtoi_errno;

/**
*	@brief      strtoi() shall convert string nptr to int out. 
*
*   @param		nptr - Input string to be converted.
*	@param		out  - The converted int.
*	@param		base - Base to interpret string in. Same range as strtol (2 to 36).
*
*		The format is the same as strtol, except that the following are inconvertible:
*		
*		- empty string
*		- leading whitespace
*		- any trailing characters that are not part of the number
*
*	@return		Indicates if the operation succeeded, or why it failed. 
*/
strtoi_errno strtoi (int *restrict out, const char *restrict nptr, 
                     int base);

#endif   /* STRTOI_H */
