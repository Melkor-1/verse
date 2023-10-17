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

#define MAX_VERSE_SIZE 4096

int
parse_response_json (const char *restrict json_response, char **restrict out)
{
    json_error_t error;
    json_t *root = json_loads(json_response, 0, &error);

    if(root) {
        /* Check if "text" exists in the JSON structure. */
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

size_t
write_memory_callback (void *content, size_t size,
                       size_t nmemb, struct mem_chunk *chunk)
{
    const size_t new_size = chunk->len + size * nmemb;
    void *const cp = realloc (chunk->ptr, new_size + 1);

    if (!cp) {
        perror ("realloc()");
        return 0;
    }
    chunk->ptr = cp;
    memcpy (chunk->ptr + chunk->len, content, size * nmemb);
    chunk->ptr[new_size] = '\0';
    chunk->len = new_size;
    return size * nmemb;
}

int
download_webpage (const struct mem_chunk *restrict chunk,
                  CURL * restrict curl, const char *restrict url)
{
    CURLcode ret; 

    if ((ret = curl_easy_setopt (curl, CURLOPT_URL, url)) != CURLE_OK 
        || (ret = curl_easy_setopt (curl, CURLOPT_WRITEFUNCTION, write_memory_callback)) != CURLE_OK
        || (ret = curl_easy_setopt (curl, CURLOPT_WRITEDATA, chunk)) != CURLE_OK
        || (ret = curl_easy_setopt (curl, CURLOPT_USERAGENT, "Verse/1.0")) != CURLE_OK
        || (ret = curl_easy_setopt (curl, CURLOPT_FOLLOWLOCATION, 1L)) != CURLE_OK
        || (ret = curl_easy_setopt (curl, CURLOPT_MAXREDIRS, 5L)) != CURLE_OK) {
        return (int) ret;
    }
    
    return (int) curl_easy_perform (curl);
}

int build_url (char **restrict url, const char *base_url, size_t base_url_size, int chapter, int verse)
{
    /*
     * Allocate space for the full url. 
     */
    *url = malloc (base_url_size * 2);

    if (!(*url)) {
        return E_ENOMEM;
    }
    snprintf (*url, base_url_size * 2,
              "%s/%d:%d/en.maududi", base_url, chapter, verse);
    return E_SUCCESS;
}
