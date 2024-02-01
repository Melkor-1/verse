#ifdef _POSIX_C_SOURCE
#undef _POSIX_C_SOURCE
#endif

#ifdef _XOPEN_SOURCE
#undef _XOPEN_SOURCE
#endif

#define _POSIX_C_SOURCE 200819L
#define _XOPEN_SOURCE	700

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "str2i.h"
#include "web_util.h"
#include "errors.h"

#define MAX_CHAPTER		114
#define MIN_CHAPTER		0
#define MIN_VERSE		0

/*
 * Each entry in the table is the maximum number of verses present in its 
 * corresponding index, which denotes the chapter number. 
 */
static const int verse_counts[] = {
    7, 286, 200, 176, 120, 165, 206, 75, 129, 109, 123, 111, 43, 52,
    99, 128, 111, 110, 98, 135, 112, 78, 118, 64, 77, 227, 93, 88, 69,
    60, 34, 30, 73, 54, 45, 83, 182, 88, 75, 85, 54, 53, 89, 59, 37,
    35, 38, 29, 18, 45, 60, 49, 62, 55, 78, 96, 29, 22, 24, 13, 14,
    11, 11, 18, 12, 12, 30, 52, 52, 44, 28, 28, 20, 56, 40, 31, 50,
    40, 46, 42, 29, 19, 36, 25, 22, 17, 19, 26, 30, 20, 15, 21, 11,
    8, 8, 19, 5, 8, 8, 11, 11, 8, 3, 9, 5, 4, 7, 3, 6, 3, 5, 4, 5, 6
};

static inline int check_args(int argc, const char *const *argv)
{
    /*
     * Sanity check. POSIX requires the invoking process to pass a non-NULL argv[0]. 
     */
    return (!argv[0]) ? E_NULL_ARGV :
        (argc != 3) ? E_INSUFFICIENT_ARGS : E_SUCCESS;
}

static int check_input(const char *const *restrict argv, int *restrict chapter,
                       int *restrict verse)
{
    const int ret_1 = str2i(chapter, argv[1], 10);
    const int ret_2 = str2i(verse, argv[2], 10);
    
    /* *INDENT-OFF* */
    return (ret_1 == STR2I_INCONVERTIBLE || ret_2 == STR2I_INCONVERTIBLE) ? E_PARSE_ERROR :
           (ret_1 == STR2I_UNDERFLOW || ret_2 == STR2I_UNDERFLOW ||
            ret_1 == STR2I_OVERFLOW || ret_2 == STR2I_OVERFLOW) ? E_INVALID_RANGE :
           (*chapter <= MIN_CHAPTER || *chapter > MAX_CHAPTER) ? E_INVALID_CHAPTER :
           (*verse <= MIN_VERSE || *verse > verse_counts [*chapter - 1]) ? E_INVALID_VERSE :
           E_SUCCESS;
    /* *INDENT-ON* */
}

int main(int argc, char **argv)
{
    const char *const *args = (const char *const *)argv;
    int status = check_args(argc, args);

    if (status != E_SUCCESS) {
        fputs(get_err_msg(status), stderr);
        return EXIT_FAILURE;
    }

    int chapter, verse;

    status = check_input(args, &chapter, &verse);

    if (status != E_SUCCESS) {
        fputs(get_err_msg(status), stderr);
        return EXIT_FAILURE;
    }

    status = handle_args(chapter, verse);
    if (status != E_SUCCESS) {
        fputs(get_err_msg(status), stderr);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
