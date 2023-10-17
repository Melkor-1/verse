#include <criterion/criterion.h>
#include <criterion/new/assert.h>

#ifndef UNIT_TESTING
#   define UNIT_TESTING
#endif

#include "../src/errors.h"
#include "../src/strtoi.h"
#include "../src/web_util.h"
#include "../src/main.c"

Test (errors_tests, get_err_msg) 
{
    /* Test with an out of range code. */
    cr_assert (not (strcmp (get_err_msg (-1), "Unknown error code.\n")));
    cr_assert (not (strcmp (get_err_msg (E_CURL_PERFORM_FAILED + 1), "Unknown error code.\n")));

    /* Test with a valid code. */
    cr_assert (not (strcmp (get_err_msg (E_CURL_PERFORM_FAILED), errors[E_CURL_PERFORM_FAILED])));
}

Test (main_tests, check_args) 
{
    /* Test with a NULL argv[0]. */
    cr_assert (eq (int, check_args (0, (const char *[]) {NULL}),  E_NULL_ARGV));

    /* Test with insufficient arguments. */
    cr_assert (eq (int, check_args (2, (const char *[]) {"verse", "3"}), E_INSUFFICIENT_ARGS));
 
    /* Test with valid arguments. */
    cr_assert (eq (int, check_args (3, (const char *[]) {"verse", "3", "3"}) == E_SUCCESS));
}

Test (main_tests, check_input) 
{
    char *argv[] = {"verse", "1", "1"};
    int verse = 0;
    int chapter = 0;

    /* Test with valid chapter and verse numbers. */
    cr_assert (eq (int, check_input (argv, &chapter, &verse), E_SUCCESS));
    cr_assert (all (eq (int, chapter, 1), eq (int, verse, 1)));

    /* Test with invalid chapter (out of numeric range). */
    char s[128];
    snprintf (s, sizeof s, "%ld", (int long) INT_MAX + 1);
    argv[1] = s;
    cr_assert (eq (int, check_input (argv, &chapter, &verse ), E_INVALID_RANGE));

    /* Test with invalid verse number (out of numeric range). */
    argv[2] = s;
    cr_assert (eq (int, check_input (argv, &chapter, &verse), E_INVALID_RANGE));

    /* Test with invalid chapter number (chapter < MIN_CHAPTER || chapter > MAX_CHAPTER). */
    snprintf (s, sizeof s, "%d", MIN_CHAPTER - 1);
    argv[1] = s;
    argv[2] = "1";
    cr_assert (eq (int, check_input (argv, &chapter, &verse), E_INVALID_CHAPTER));

    snprintf (s, sizeof s, "%d", MAX_CHAPTER + 1);
    argv[1] = s;
    cr_assert (eq (int, check_input (argv, &chapter, &verse), E_INVALID_CHAPTER));

    /* Test with invalid verse number (verse < MIN_VERSE || verse > MAX_VERSE). */
    snprintf (s, sizeof s, "%d", MIN_VERSE - 1);
    argv[1] = "1";
    argv[2] = s;
    cr_assert (eq (int, check_input (argv, &chapter, &verse), E_INVALID_VERSE));

    snprintf (s, sizeof s, "%d", MAX_VERSE + 1);
    argv[2] = s;
    cr_assert (eq (int, check_input (argv, &chapter, &verse), E_INVALID_VERSE));

    /* Test with non-numeric chapter. */
    argv[1] = "abcd";
    argv[2] = "1";
    cr_assert (eq (int, check_input (argv, &chapter, &verse), E_PARSE_ERROR));

    /* Test with non-numeric verse. */
    argv[2] = argv[1];
    argv[1] = "1";
    cr_assert (eq (int, check_input (argv, &chapter, &verse), E_PARSE_ERROR));
}

Test (strtoi, strtoi)
{
    int i = 0;
    char s[256];

    /* Test a positive integer string. */
    cr_assert (eq (int, strtoi (&i, "39", 10), STRTOI_SUCCESS));
    cr_assert (eq (int, i, 39));

    /* Test a negative integer string. */
    cr_assert (eq (int, strtoi (&i, "-38", 10), STRTOI_SUCCESS));
    cr_assert (eq (int, i, -38));

    /* Test zero as a string. */
    cr_assert (eq (int, strtoi (&i, "0", 10), STRTOI_SUCCESS));
    cr_assert (not (i));

    /* Test a non-digit string. */
    cr_assert (eq (int, strtoi (&i, "abcsda", 10), STRTOI_INCONVERTIBLE));
    
    /* Test conversion with different base values. */
    cr_assert (eq (int, strtoi (&i, "0x11", 16), STRTOI_SUCCESS));
    cr_assert (eq (int, i, 17));

    /* Test an empty string. */
    cr_assert (eq (int, strtoi (&i, "", 10), STRTOI_INCONVERTIBLE));

    /* Test a string with just a sign character ('+' or '-'). */
    cr_assert (eq (int, strtoi (&i, "-", 10), STRTOI_INCONVERTIBLE));
    cr_assert (eq (int, strtoi (&i, "+", 10), STRTOI_INCONVERTIBLE));

    /* Test an integer larger than INT_MAX.  
     * 'if' needed to avoid undefined behaviour 
     * on 'INT_MAX + 1' if INT_MAX == LONG_MAX. 
     */
    if (INT_MAX < LONG_MAX) {
        snprintf (s, sizeof s, "%ld", (int long) INT_MAX + 1L);
        cr_assert (eq (int, strtoi (&i, s, 10), STRTOI_OVERFLOW));
    }

    /* Test an integer smaller than INT_MIN. */
    if (LONG_MIN < INT_MIN) {
        snprintf (s, sizeof s, "%ld", (int long) INT_MIN - 1L);
        cr_assert (eq (int, strtoi (&i, s, 10), STRTOI_UNDERFLOW));
    }
    
    /* Test a string with leading whitespace. */
    cr_assert (eq (int, strtoi (&i, " ab3", 10), STRTOI_INCONVERTIBLE));

    /* Test a string with trailing whitespace. */
    cr_assert (eq (int, strtoi (&i, "22 ", 10), STRTOI_INCONVERTIBLE));

    /* Test a string with leading garbage characters. */
    cr_assert (eq (int, strtoi (&i, "ab312", 10), STRTOI_INCONVERTIBLE));

    /* Test a string with trailing garbage characters. */
    cr_assert (eq (int, strtoi (&i, "382ac", 10), STRTOI_INCONVERTIBLE));

    /* Test INT_MIN. */
    snprintf (s, sizeof s, "%d", INT_MIN);
    cr_assert (eq (int, strtoi (&i, s, 10), STRTOI_SUCCESS));
    cr_assert (eq (int, i, INT_MIN));
    
    /* Test INT_MAX. */
    snprintf (s, sizeof s, "%d", INT_MAX);
    cr_assert (eq (int, strtoi (&i, s, 10), STRTOI_SUCCESS));
    cr_assert (eq (int, i, INT_MAX));

    /* Test passing a NULL pointer as the string. */
    cr_assert (eq (int, strtoi (&i, NULL, 10), STRTOI_INCONVERTIBLE));
}

Test (web_util, parse_response_json)
{
    /* Test with an invalid JSON response. */
    static const char *invalid_response = "invalid_json";
    char *result = NULL;
    int rc = parse_response_json(invalid_response, &result);

    cr_assert (eq (int, rc, E_UNKNOWN));
    cr_assert (not (result));

    /* Test with valid JSON response but missing "data" key. */
    static const char *missing_data_key_response = "{\"no_data_key\": {\"text\": \"(1:1) In the name of Allah, the Most Gracious, the Most Merciful.\"}}";
    rc = parse_response_json(missing_data_key_response, &result);

    cr_assert (eq (int, rc, E_UNKNOWN));
    cr_assert (not (result));

    /* Test with valid JSON response. */
    static const char valid_response = "{\"data\": {\"text\": \"(1:1) In the name of Allah, the Most Gracious, the Most Merciful.\"}}";
    rc = parse_response_json (valid_response, &result);

    cr_assert (eq (int, rc, E_SUCCESS));
    cr_assert (result);
    cr_assert (eq (int, strcmp ("(1:1) In the name of Allah, the Most Gracious, the Most Merciful.", result)));
    free (result);
}

Test (config_tests, build_url)
{
    char *url = NULL;
    char base_url[128] = "https://stackoverflow.com/api/";
    size_t base_url_size = sizeof (base_url);
    int chapter = 2;
    int verse = 3;

    int rc = build_url (&url, base_url, base_url_size, chapter, verse);

    cr_assert (eq (int, rc, E_SUCCESS);
    cr_assert (not (url));
    
    snprintf (base_url, base_url_size, "%d:%d/en.maududi", chapter, verse);
    cr_assert (not (strcmp (url, base_url));
    free (base_url);
}
