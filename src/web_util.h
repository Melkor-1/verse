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
int parse_response_json (const char *restrict json_response, char **restrict out);

/**
*   @brief      A callback function for curl_easy_perform(). Stores the downloaded
*               web content to the mem_chunk struct as it arrives.
*   @param      Refer to https://curl.se/libcurl/c/CURLOPT_WRITEFUNCTION.html for a 
*               detailed explanation about the parameters.
*   @return     The number of bytes read, or 0 on a memory allocation failure.
*/

size_t write_memory_callback (void *content, size_t size,
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
int download_webpage (const struct mem_chunk *restrict chunk,
                      CURL * restrict curl, const char *restrict url);


/**
*   @brief      build_url() shall build a complete URL using the base URL, chapter, and verse information.
*
*   @param      url             - A pointer to a string that will hold the complete URL.
*   @param      chapter         - The chapter number of the desired verse.
*   @param      base_url        - The base URL to which to which chapter and verse information will be appended. 
*   @param      base_url_size   - The size of the base URL. 
*   @param      verse           - The verse number within the given chapter.
*
*   @return     An error code indicating the success or failure of building the URL.
*               If successful, the complete URL will be stored in the 'url' parameter.
*/
int build_url (char **restrict url, const char *restrict base_url, size_t base_url_size, int chapter, int verse);

#endif   /* WEB_UTIL_H */
