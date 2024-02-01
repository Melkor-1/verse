#ifndef STR2I_H
#define STR2I_H

typedef enum {
    STR2I_SUCCESS,
    STR2I_OVERFLOW,
    STR2I_UNDERFLOW,
    STR2I_INCONVERTIBLE
} str2i_errno;

/**
*	@brief      str2i() shall convert string nptr to int out. 
*
*   @param		nptr - Input string to be converted.
*	@param		out  - The converted int.
*	@param		base - Base to interpret string in. Same range as strtol (0 and 2 to 36).
*
*		The format is the same as strtol, except that the following are inconvertible:
*		
*		- leading whitespace
*		- any trailing characters that are not part of the number
*
*	@return		Indicates if the operation succeeded, or why it failed. 
*/
str2i_errno str2i(int *restrict out, const char *restrict nptr, int base);

#endif                          /* STR2I_H */
