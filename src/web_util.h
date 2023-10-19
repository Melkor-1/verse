#ifndef WEB_UTIL_H
#define WEB_UTIL_H

#include <stddef.h>
#include <curl/curl.h>

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
int parse_response_json(const char *restrict json_response,
                        char **restrict out);

/**
*   @brief      A callback function for curl_easy_perform(). Stores the downloaded
*               web content to the mem_chunk struct as it arrives.
*   @param      Refer to https://curl.se/libcurl/c/CURLOPT_WRITEFUNCTION.html for a 
*               detailed explanation about the parameters.
*   @return     The number of bytes read, or 0 on a memory allocation failure.
*/

size_t write_memory_callback(void *content, size_t size,
                             size_t nmemb, struct mem_chunk *chunk);

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
int download_webpage(const struct mem_chunk *restrict chunk,
                     CURL * restrict curl, const char *restrict url);

#endif                          /* WEB_UTIL_H */
