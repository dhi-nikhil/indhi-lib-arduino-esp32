#ifndef JSON_PARSER_H_
#define JSON_PARSER_H_

#include "freertos/FreeRTOS.h"
#include "string"
#include "string.h"
#include "cJSON.h"

/**
 * @brief JSON Single level parser
 *
 */
class JSONParser
{
private:
protected:
public:
    /**
     * @brief Construct a new JSONParser object
     *
     */
    JSONParser();
    /**
     * @brief Destroy the JSONParser object
     *
     */
    ~JSONParser();

    /**
     * @brief JSON Parse by key
     *
     * @param output Integet output
     * @param key key to fetch
     * @param input input string from which key will be fetched
     * @return esp_err_t success or failed
     */
    static esp_err_t JSONParserParseValueByKey(int * output, std::string key, std::string input);
    /**
     * @brief JSON Parse by key
     *
     * @param output String output
     * @param key key to fetch
     * @param input input string from which key will be fetched
     * @return esp_err_t success or failed
     */
    static esp_err_t JSONParserParseValueByKey(std::string * output, std::string key, std::string input);
    /**
     * @brief JSON Parse by key
     *
     * @param output String output
     * @param key key to fetch
     * @param input input string from which key will be fetched
     * @return esp_err_t success or failed
     */
    static esp_err_t JSONParserParseValueByKey(char ** output, std::string key, std::string input);
};

#endif
