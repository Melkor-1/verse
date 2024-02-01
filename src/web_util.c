#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 700
#endif

#include "web_util.h"
#include "errors.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <jansson.h>
#include <curl/curl.h>

#define MAX_VERSE_SIZE 4096

#define BASE_URL        "http://api.alquran.cloud/v1/ayah/%d:%d/en.maududi"
#define MAX_URL_SIZE    128


/*
 * A struct to hold the contents of the downloaded web page. 
 */
struct mem_chunk {
    char *ptr;
    size_t len;
};

/**
*   @brief      Parses a JSON response to extract a specific verse.
*
*   @param      json_responose - A JSON response string containing verse data.
*   @param      out            - A pointer to a string where the parsed verse will be stored.
*   
*   @return     An error code indicating the success or failure of the parsing process.
*               If successful, the parsed verse will be stored in the 'out' parameter.
*/
static int parse_response_json(const char *restrict json_response, char **restrict out)
{
    json_error_t error;
    json_t *const root = json_loads(json_response, 0, &error);

    if (root) {
        /*
         * Check if "text" exists in the JSON structure. 
         */
        const json_t *const data = json_object_get(root, "data");
        const json_t *const text = json_object_get(data, "text");

        if (data && text && json_is_string(text)) {
            *out = strdup(json_string_value(text));
        } else {
            json_decref(root);
            return E_UNKNOWN;
        }
    } else {
        fputs(error.text, stderr);
        return E_UNKNOWN;
    }

    json_decref(root);
    return E_SUCCESS;
}

/**
*   @brief      A callback function for curl_easy_perform(). Stores the downloaded
*               web content to the mem_chunk struct as it arrives.
*   @param      Refer to https://curl.se/libcurl/c/CURLOPT_WRITEFUNCTION.html for a 
*               detailed explanation about the parameters.
*   @return     The number of bytes read, or 0 on a memory allocation failure.
*/
static size_t
write_memory_callback(void *content, size_t size,
                      size_t nmemb, struct mem_chunk *chunk)
{
    const size_t new_size = chunk->len + size * nmemb;
    void *const cp = realloc(chunk->ptr, new_size + 1);

    if (!cp) {
        perror("realloc()");
        return 0;
    }
    chunk->ptr = cp;
    memcpy(chunk->ptr + chunk->len, content, size * nmemb);
    chunk->ptr[new_size] = '\0';
    chunk->len = new_size;
    return size * nmemb;
}

/**
*   @brief       download_webpage() shall download the contents of a web page specified
*                by the URL using libcurl.
*   
*   @param       chunk - A struct to hold the downloaded content.
*   @param       curl  - A libcurl handle for performing the download.
*   @param       url   - The URL of the web page to download.
*   
*   @return     CURLE_OK on success, or a libcurl error code on failure.
*/
static int
download_webpage(const struct mem_chunk *restrict chunk,
                 CURL * restrict curl, const char *restrict url)
{
    CURLcode ret;

    /* *INDENT-OFF* */
    if ((ret = curl_easy_setopt(curl, CURLOPT_URL, url)) != CURLE_OK
        || (ret = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_memory_callback)) != CURLE_OK
        || (ret = curl_easy_setopt(curl, CURLOPT_WRITEDATA, chunk)) != CURLE_OK
        || (ret = curl_easy_setopt(curl, CURLOPT_USERAGENT, "Verse/1.0")) != CURLE_OK
        || (ret = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L)) != CURLE_OK
        || (ret = curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 5L)) != CURLE_OK) {
        return (int) ret;
    }
    /* *INDENT-ON* */

    return (int) curl_easy_perform(curl);
}

int handle_args(int chapter, int verse)
{
    char url[MAX_URL_SIZE];

    snprintf(url, sizeof (url), BASE_URL, chapter, verse);

    struct mem_chunk chunk = { 0 };
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
