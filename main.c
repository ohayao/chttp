#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <curl/easy.h>

struct MemoryStruct
{
    char *memory;
    size_t size;
};

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;
    mem->memory = (char *)realloc(mem->memory, mem->size + realsize + 1);
    if (mem->memory == NULL)
    {
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;
    return realsize;
}

char *str_replace(char *src, char *rep, char *with)
{
    char *index;
    char *result, *tmp, *next;
    int count = 0, len_front;
    int len_with = strlen(with);
    int len_rep = strlen(rep);
    next = src;
    for (count = 0; tmp = strstr(next, rep); ++count)
    {
        next = tmp + len_rep;
    }
    tmp = result = (char *)malloc(strlen(src) + count * (len_rep - len_with) + 1);
    if (!result)
        return NULL;
    while (count--)
    {
        index = strstr(src, rep);
        len_front = index - src;
        tmp = strncpy(tmp, src, len_front) + len_front;
        tmp = strcpy(tmp, with) + len_with;
        src += len_front + len_rep; // move to next "end of rep"
    }
    strcpy(tmp, src);
    return result;
}

void aCurl()
{
    struct MemoryStruct chunk;
    chunk.memory = (char *)malloc(1);
    chunk.size = 0;

    CURLcode res;
    CURL *curl;
    int responseCode = 0;
    curl = curl_easy_init();
    if (curl == NULL)
    {
        return;
    }
    char *url = "https://xxx.domain.com/post";
    char *postData = "";
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

    res = curl_easy_perform(curl);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responseCode);
    printf("result:%d >>>> responseCode:%d\n", res, responseCode);
    if (res != CURLE_OK)
    {
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
        return;
    }
    else
    {
        printf("返回结果:\t%s\n", chunk.memory);
    }
    curl_easy_cleanup(curl);
}

int main()
{
    aCurl();
    return 0;
}