#ifndef HTTP_CLIENT_SSL_CERT_H_
#define HTTP_CLIENT_SSL_CERT_H_

#include "SSLUtils.h"

/**
 * @brief HTTPClient for the SSLUtils
 *
 */
class HTTPClientSSLCert :public SSLUtils
{
public:
    /**
     * @brief Construct a new HTTPClientSSLCert object for the certificate
     *
     */
    HTTPClientSSLCert();
};

#endif
