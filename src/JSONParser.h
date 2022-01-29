#ifndef JSON_PARSER_H_
#define JSON_PARSER_H_

#include "freertos/FreeRTOS.h"
#include "string"
#include "string.h"
#include "cJSON.h"

class JSONParser
{
private:
protected:
public:
    JSONParser();
    ~JSONParser();

    static esp_err_t JSONParserParseValueByKey(int * output, std::string key, std::string input);
    static esp_err_t JSONParserParseValueByKey(std::string * output, std::string key, std::string input);
    static esp_err_t JSONParserParseValueByKey(char ** output, std::string key, std::string input);
};

#endif
