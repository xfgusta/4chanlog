#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <json-c/json.h>

/* just for fun */

struct memory {  
    char *memory;
    size_t size;
};

size_t callback(void *, size_t, size_t, void *);
void puts_link(char *, const char *, struct json_object *);

int main(int argc, char **argv) {
    if(argc != 2) {
        fprintf(stderr, "Use: %s <board>\n", argv[0]);
        return 1;
    }

    char *board = argv[1];
    size_t n = strlen(board) + 42;
    
    char *url = malloc(n);
    if(!url) {
        fprintf(stderr, "Not enough memory (malloc returned NULL)\n");
        return 1;
    }

    CURL *curl = curl_easy_init();
    if(!curl) {
        fprintf(stderr, "Cannot init curl\n");
        free(url);
        return 1;
    }
    
    struct memory chunk = {
        .memory = NULL,
        .size = 0
    };
    
    snprintf(url, n, "https://boards.4channel.org/%s/catalog.json", board);

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &chunk);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
    
    CURLcode res = curl_easy_perform(curl);
    if(res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
            curl_easy_strerror(res));
        curl_easy_cleanup(curl);
        if(chunk.memory)
            free(chunk.memory);
        free(url);
        return 1;
    }
    
    curl_easy_cleanup(curl);
    free(url);
    
    struct json_object *json, *arr, *th, *rp;

    json = json_tokener_parse(chunk.memory);
    if(!json) {
        fprintf(stderr, "Cannot parse catalog.json\n");
        free(chunk.memory);
        return 1;
    }
    
    size_t json_len = json_object_array_length(json);
    for(size_t i = 0; i < json_len; i++) {
        th = json_object_object_get(
            json_object_array_get_idx(json, i),
            "threads"
        );
    
        size_t th_len = json_object_array_length(th);
        for(size_t j = 0; j < th_len; j++) {
            arr = json_object_array_get_idx(th, j);
            
            const char *file = json_object_to_json_string(
                json_object_object_get(arr, "tim")
            );
            if(!file || !strcmp(file, "null"))
                break;
            else
                puts_link(board, file, arr);

            rp = json_object_object_get(arr, "last_replies");
            if(!rp)
                break;
    
            size_t rp_len = json_object_array_length(rp);
            for(size_t k = 0; k < rp_len; k++) {
                arr = json_object_array_get_idx(rp, k);

                const char *file = json_object_to_json_string(
                    json_object_object_get(arr, "tim")
                );
                if(!file || !strcmp(file, "null"))
                    break;
                else
                    puts_link(board, file, arr);
            }
        }
    }

    json_object_put(json);
    free(chunk.memory);
    
    return 0;
}

size_t callback(void *data, size_t size, size_t nmemb, void *userp) {
    size_t real_size = size * nmemb;
    struct memory *mem = (struct memory *) userp;
    
    char *ptr = realloc(mem->memory, mem->size + real_size + 1);
    if(!ptr) {
        fprintf(stderr, "Not enough memory (realloc returned NULL)\n");
        return 0;
    }
    
    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), data, real_size);
    mem->size += real_size;
    mem->memory[mem->size] = 0;
    
    return real_size;
}

void puts_link(char *board, const char *file, struct json_object *obj) {
    printf("https://i.4cdn.org/%s/%s%s (%sx%s)\n",
        board,
        file,
        json_object_get_string(
            json_object_object_get(obj, "ext")
        ),
        json_object_get_string(
            json_object_object_get(obj, "w")
        ),
        json_object_get_string(
            json_object_object_get(obj, "h")
        )
    );
}
