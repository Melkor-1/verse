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

#include "strtoi.h"
#include "web_util.h"
#include "errors.h"

#define BASE_URL        "http://api.alquran.cloud/v1/ayah/%d:%d/en.maududi"
#define MAX_URL_SIZE    128

#define MAX_CHAPTER		114
#define MIN_CHAPTER		0
#define MAX_VERSE		6236
#define MIN_VERSE		0

#define INIT_MEM_CHUNK(address, size) \
	{ .ptr = address, .len = size }

/*
 * Each entry in the table is the maximum number of verses present in its 
 * corresponding index, which denotes the chapter number. 
 */
static const int verse_limits[] = {
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

static int check_input(const char *const *restrict argv, int *chapter,
                       int *restrict verse)
{
    const int ret_1 = strtoi(chapter, argv[1], 10);
    const int ret_2 = strtoi(verse, argv[2], 10);

    /* *INDENT-OFF* */
    return (ret_1 == STRTOI_INCONVERTIBLE || ret_2 == STRTOI_INCONVERTIBLE) ? E_PARSE_ERROR :
           (ret_1 == STRTOI_UNDERFLOW || ret_2 == STRTOI_UNDERFLOW ||
            ret_1 == STRTOI_OVERFLOW || ret_2 == STRTOI_OVERFLOW) ? E_INVALID_RANGE :
           (*chapter <= MIN_CHAPTER || *chapter > MAX_CHAPTER) ? E_INVALID_CHAPTER :
           (*verse <= MIN_VERSE || *verse > verse_limits [*chapter - 1]) ? E_INVALID_VERSE :
           E_SUCCESS;
    /* *INDENT-ON* */
}

static int handle_args(int chapter, int verse)
{
    char url[MAX_URL_SIZE];

    snprintf(url, sizeof (url), BASE_URL, chapter, verse);

    struct mem_chunk chunk = INIT_MEM_CHUNK(0, 0);
    CURL *const curl = curl_easy_init();

    if (!curl) {
        return E_CURL_INIT_FAILED;
    }

    int rc = download_webpage(&chunk, curl, url);

    if (rc != CURLE_OK) {
        curl_easy_cleanup(curl);
        free(chunk.ptr);
        return E_CURL_PERFORM_FAILED;
    } else {
        char *result = NULL;

        rc = parse_response_json(chunk.ptr, &result);

        if (rc != E_SUCCESS) {
            curl_easy_cleanup(curl);
            free(chunk.ptr);
            return rc;
        } else {
            printf("(%d:%d) %s\n", chapter, verse, result);
        }
        free(result);
    }
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    free(chunk.ptr);
    return E_SUCCESS;
}

int main(int argc, char **argv)
{
    int status = check_args(argc, argv);

    if (status != E_SUCCESS) {
        fputs(get_err_msg(status), stderr);
        return EXIT_FAILURE;
    }

    int chapter, verse;

    status = check_input(argv, &chapter, &verse);

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
