#define LOG_LOCAL_LEVEL ESP_LOG_NONE

#include "JSONParser.h"

static const char * TAG = "JSONParser";

JSONParser::JSONParser()
{

}

esp_err_t JSONParser::JSONParserParseValueByKey(int * output, std::string key, std::string input)
{
    esp_err_t err = ESP_OK;
    cJSON * p1, * p2;
    p1 = cJSON_Parse(input.c_str());
    if (p1 == NULL)
    {
        err = ESP_FAIL;
        goto end;
    }

    p2 = cJSON_GetObjectItem(p1, key.c_str());
    if (p2 == NULL)
    {
        err = ESP_FAIL;
        goto end;
    }

    *output = p2->valuedouble;
end:
    cJSON_Delete(p1);
    return err;
}

esp_err_t JSONParser::JSONParserParseValueByKey(std::string * output, std::string key, std::string input)
{
    esp_err_t err = ESP_OK;
    cJSON * p1, * p2;
    p1 = cJSON_Parse(input.c_str());
    if (p1 == NULL)
    {
        err = ESP_FAIL;
        goto end;
    }

    p2 = cJSON_GetObjectItem(p1, key.c_str());
    if (p2 == NULL)
    {
        err = ESP_FAIL;
        goto end;
    }

    *output = std::string(p2->valuestring);
end:
    cJSON_Delete(p1);
    return err;
}

esp_err_t JSONParser::JSONParserParseValueByKey(char ** output, std::string key, std::string input)
{
    esp_err_t err = ESP_OK;
    cJSON * p1, * p2;
    p1 = cJSON_Parse(input.c_str());
    if (p1 == NULL)
    {
        err = ESP_FAIL;
        goto end;
    }

    p2 = cJSON_GetObjectItem(p1, key.c_str());
    if (p2 == NULL)
    {
        err = ESP_FAIL;
        goto end;
    }

    strcpy(*output, p2->valuestring);
end:
    cJSON_Delete(p1);
    return err;
}

JSONParser::~JSONParser()
{

}
