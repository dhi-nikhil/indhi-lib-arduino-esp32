#define LOG_LOCAL_LEVEL ESP_LOG_NONE

#include "Utils.h"

static const char * TAG = "Utils";

// namespace Utils
// {
    // int Utils::Base64DecodedLength(size_t length)
    // {
    //     int numEq = 0;
    //     int n = (int)data.size();

    //     for (std::string::const_reverse_iterator it = data.rbegin(); *it == '='; ++it)
    //     {
    //         ++numEq;
    //     }
    //     return ((6 * n) / 8) - numEq;
    // }

int Utils::Base64DecodedLength(const std::string & data)
{
    int numEq = 0;
    int n = (int) data.size();

    for (std::string::const_reverse_iterator it = data.rbegin(); *it == '='; ++it)
    {
        ++numEq;
    }
    return ( ( 6 * n ) / 8 ) - numEq;
}

unsigned char Utils::Base64Lookup(unsigned char ch)
{
    if (ch >= 'A' && ch <= 'Z')
        return ch - 'A';
    if (ch >= 'a' && ch <= 'z')
        return ch - 71;
    if (ch >= '0' && ch <= '9')
        return ch + 4;
    if (ch == '+')
        return 62;
    if (ch == '/')
        return 63;
    return 255;
};

int Utils::Base64EncodedLength(size_t length)
{
    return ( length + 2 - ( ( length + 2 ) % 3 ) ) / 3 * 4;
}

int Utils::Base64EncodedLength(const std::string & data)
{
    return Base64EncodedLength(data.length());
}

void Utils::A3toA4(unsigned char * a4, unsigned char * a3)
{
    a4[0] = ( a3[0] & 0xFC ) >> 2;
    a4[1] = ( ( a3[0] & 0x03 ) << 4 ) + ( ( a3[1] & 0xF0 ) >> 4 );
    a4[2] = ( ( a3[1] & 0x0F ) << 2 ) + ( ( a3[2] & 0xC0 ) >> 6 );
    a4[3] = ( a3[2] & 0x3F );
}

void Utils::A4toA3(unsigned char * a3, unsigned char * a4)
{
    a3[0] = ( a4[0] << 2 ) + ( ( a4[1] & 0x30 ) >> 4 );
    a3[1] = ( ( a4[1] & 0xf ) << 4 ) + ( ( a4[2] & 0x3C ) >> 2 );
    a3[2] = ( ( a4[2] & 0x3 ) << 6 ) + a4[3];
}

bool Utils::Base64Decode(std::string * destination, const std::string & source)
{
    int i = 0, j = 0;
    size_t decodedLength = 0;
    unsigned char a3[3];
    unsigned char a4[4];

    int sourceLength = source.size();
    std::string::const_iterator input = source.begin();

    destination->resize(Base64DecodedLength(source));

    while (sourceLength--)
    {
        if (*input == '=')
        {
            break;
        }

        a4[i++] = *( input++ );
        if (i == 4)
        {
            for (i = 0; i < 4; i++)
            {
                a4[i] = Base64Lookup(a4[i]);
            }

            A4toA3(a3, a4);

            for (i = 0; i < 3; i++)
            {
                ( *destination )[decodedLength++] = a3[i];
            }

            i = 0;
        }
    }

    if (i)
    {
        for (j = i; j < 4; j++)
        {
            a4[j] = '\0';
        }

        for (j = 0; j < 4; j++)
        {
            a4[j] = Base64Lookup(a4[j]);
        }

        A3toA4(a3, a4);

        for (j = 0; j < i - 1; j++)
        {
            ( *destination )[decodedLength++] = a3[j];
        }
    }

    return ( decodedLength == destination->size() );
}

bool Utils::Base64Encode(std::string * destination, const std::string & source)
{
    int i = 0, j = 0;
    size_t encodedLength = 0;
    unsigned char a3[3];
    unsigned char a4[4];

    destination->resize(Base64EncodedLength(source));

    int sourceLength = source.size();
    std::string::const_iterator input = source.begin();

    while (sourceLength--)
    {
        a3[i++] = *( input++ );
        if (i == 3)
        {
            A3toA4(a4, a3);

            for (i = 0; i < 4; i++)
            {
                ( *destination )[encodedLength++] = Base64AlphabetMap[a4[i]];
            }

            i = 0;
        }
    }

    if (i)
    {
        for (j = i; j < 3; j++)
        {
            a3[j] = '\0';
        }

        A3toA4(a4, a3);

        for (j = 0; j < i + 1; j++)
        {
            ( *destination )[encodedLength++] = Base64AlphabetMap[a4[j]];
        }

        while (( i++ < 3 ))
        {
            ( *destination )[encodedLength++] = '=';
        }
    }

    return ( encodedLength == destination->size() );
}

void Utils::dumpinfo()
{
    // esp_chip_info_t chipInfo;
    // esp_chip_info(&chipInfo);
    // ESP_LOGI(TAG, "Free heap: %d", ::heap_caps_get_free_size(MALLOC_CAP_8BIT));
    // ESP_LOGI(TAG, "Chip Info: Model: %d, cores: %d, revision: %d", chipInfo.model, chipInfo.cores, chipInfo.revision);
    // ESP_LOGI(TAG, "ESP-IDF version: %s", ::esp_get_idf_version());
}

bool Utils::EndsWith(std::string data, char ch)
{
    if (data.empty())
    {
        return false;
    }
    if (data.at(data.length() - 1) == ch)
    {
        return true;
    }
    return false;
}

const char * Utils::ErrorToString(esp_err_t esp_err)
{
    return "";
}

// const char* Utils::WiFiErrorToString(esp_err_t esp_err)
// {

// }

// void Utils::HexDump(const uint8_t *data, uint32_t length)
// {
// }

std::string Utils::IpToString(uint8_t * ip)
{
    auto size = 16;
    char * charIp = (char *) malloc(size);
    snprintf(charIp, size, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
    std::string stringIp(charIp);
    free(charIp);
    return stringIp;
}

std::vector<std::string> Utils::split(std::string data, char delimiter)
{
    std::vector<std::string> subStrings;
    std::size_t current, previous = 0;
    current = data.find(delimiter);
    while (current != std::string::npos)
    {
        subStrings.push_back(Trim(data.substr(previous, current - previous)));
        previous = current + 1;
        current = data.find(delimiter, previous);
    }
    subStrings.push_back(Trim(data.substr(previous, current - previous)));
    return subStrings;
}

// std::string Utils::ToLower(std::string &data)
// {
//     std::transform(data.begin(), data.end(), data.begin(), ::tolower);
//     return data;
// }

std::string Utils::Trim(const std::string & data)
{
    size_t first = data.find_first_not_of(' ');
    if (std::string::npos == first)
        return data;
    size_t last = data.find_last_not_of(' ');
    return data.substr(first, ( last - first + 1 ));
}

esp_err_t Utils::StringToHex(std::string input, unsigned char * output)
{
    for (int i = 0; i < input.length(); i++)
    {
        char temp1 = 0;
        char temp2 = 0;

        temp1 = *( input.c_str() + ( i * 2 ) );
        temp2 = *( input.c_str() + ( i * 2 + 1 ) );

        if (temp1 >= 48 && temp1 < 58)
            temp1 = temp1 - 48;
        else if (temp1 >= 'a' && temp1 <= 'z')
            temp1 = temp1 - 97 + 10;
        else if (temp1 >= 'A' && temp1 <= 'Z')
            temp1 = temp1 - 65 + 10;
        else
            return ESP_FAIL;

        if (temp2 >= 48 && temp2 < 58)
            temp2 = temp2 - 48;
        else if (temp2 >= 'a' && temp2 <= 'z')
            temp2 = temp2 - 97 + 10;
        else if (temp2 >= 'A' && temp2 <= 'Z')
            temp2 = temp2 - 65 + 10;
        else
            return ESP_FAIL;

        *( output + i ) = ( temp1 << 4 ) | temp2;
    }
    return ESP_OK;
}

esp_err_t Utils::HexToString(unsigned char * input, int length, std::string * output)
{
    char temp[3] = { 0 };
    while (length--)
    {
        sprintf(&temp[0], "%02x", *input);
        if (temp[0] == '\0')
            return ESP_ERR_INVALID_ARG;
        // logfi("%c %c", (__string - 2), (__string - 1));
        input++;
        *output += std::string(temp, 2);
        temp[0] = 0;
        temp[1] = 0;
    }
    output = NULL;
    return ESP_OK;
}

bool Utils::IsHex(int hex)
{
    return ( hex >= '0' && hex <= '9' ) ||
        ( hex >= 'a' && hex <= 'f' ) ||
        ( hex >= 'A' && hex <= 'F' );
}

esp_err_t Utils::URLDecode(const char * input, char * output)
{
    char * o;
    const char * end = input + strlen(input);
    unsigned int c;

    for (o = output; input <= end; o++)
    {
        c = *input++;
        if (c == '+')
        {
            c = ' ';
        }
        else if (c == '%' && ( !IsHex(*input++) || !IsHex(*input++) || !sscanf(input - 2, "%2x", &c) ))
        {
            return -1;
        }
        if (output)
        {
            *o = c;
        }
    }
    return ( o - output );
}
