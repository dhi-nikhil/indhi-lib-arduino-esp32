#define LOG_LOCAL_LEVEL ESP_LOG_NONE

#include "SSLUtils.h"
#include <stdlib.h>
#include <string.h>

static const char * TAG = "SSL";

// namespace Idf
// {
//     namespace SSL
//     {
SSLUtils::SSLUtils()
{
    mCertificate = nullptr;
    mKey = nullptr;
}

SSLUtils::~SSLUtils()
{
}

void SSLUtils::SetCertificate(std::string certificate)
{
    size_t length = certificate.length();
    mCertificate = (char *) malloc(length + 1);
    memcpy(mCertificate, certificate.data(), length);
    mCertificate[length] = '\0';
}

char * SSLUtils::GetCertificate()
{
    return mCertificate;
}

void SSLUtils::SetKey(std::string key)
{
    size_t length = key.length();
    mKey = (char *) malloc(length + 1);
    memcpy(mKey, key.data(), length);
    mKey[length] = '\0';
}

char * SSLUtils::GetKey()
{
    return mKey;
}
//     } // namespace SSL
// } // namespace Idf
