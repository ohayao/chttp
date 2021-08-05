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
    char *postData = "{\"timestamp\":1627974279,\"csr\":\"-----BEGIN CERTINu6GY\\n-----END ---\\n\"}";

    struct curl_slist *header = NULL;
    header = curl_slist_append(header, "token: DVIc3StJ7Jxzj6BSy3wtwUVk4rn6U7xestkxqzCtfq8=");
    header = curl_slist_append(header, "content-type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header);
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