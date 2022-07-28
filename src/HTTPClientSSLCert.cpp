#define LOG_LOCAL_LEVEL ESP_LOG_NONE

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include "HTTPClientSSLCert.h"

static const char * TAG = "HTTPClientSSLCert";

HTTPClientSSLCert::HTTPClientSSLCert() :SSLUtils()
{
    SetCertificate(
        "-----BEGIN CERTIFICATE-----" "\n"\
        "MIIDSjCCAjKgAwIBAgIQRK+wgNajJ7qJMDmGLvhAazANBgkqhkiG9w0BAQUFADA/" "\n"\
        "MSQwIgYDVQQKExtEaWdpdGFsIFNpZ25hdHVyZSBUcnVzdCBDby4xFzAVBgNVBAMT" "\n"\
        "DkRTVCBSb290IENBIFgzMB4XDTAwMDkzMDIxMTIxOVoXDTIxMDkzMDE0MDExNVow" "\n"\
        "PzEkMCIGA1UEChMbRGlnaXRhbCBTaWduYXR1cmUgVHJ1c3QgQ28uMRcwFQYDVQQD" "\n"\
        "Ew5EU1QgUm9vdCBDQSBYMzCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEB" "\n"\
        "AN+v6ZdQCINXtMxiZfaQguzH0yxrMMpb7NnDfcdAwRgUi+DoM3ZJKuM/IUmTrE4O" "\n"\
        "rz5Iy2Xu/NMhD2XSKtkyj4zl93ewEnu1lcCJo6m67XMuegwGMoOifooUMM0RoOEq" "\n"\
        "OLl5CjH9UL2AZd+3UWODyOKIYepLYYHsUmu5ouJLGiifSKOeDNoJjj4XLh7dIN9b" "\n"\
        "xiqKqy69cK3FCxolkHRyxXtqqzTWMIn/5WgTe1QLyNau7Fqckh49ZLOMxt+/yUFw" "\n"\
        "7BZy1SbsOFU5Q9D8/RhcQPGX69Wam40dutolucbY38EVAjqr2m7xPi71XAicPNaD" "\n"\
        "aeQQmxkqtilX4+U9m5/wAl0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNV" "\n"\
        "HQ8BAf8EBAMCAQYwHQYDVR0OBBYEFMSnsaR7LHH62+FLkHX/xBVghYkQMA0GCSqG" "\n"\
        "SIb3DQEBBQUAA4IBAQCjGiybFwBcqR7uKGY3Or+Dxz9LwwmglSBd49lZRNI+DT69" "\n"\
        "ikugdB/OEIKcdBodfpga3csTS7MgROSR6cz8faXbauX+5v3gTt23ADq1cEmv8uXr" "\n"\
        "AvHRAosZy5Q6XkjEGB5YGV8eAlrwDPGxrancWYaLbumR9YbK+rlmM6pZW87ipxZz" "\n"\
        "R8srzJmwN0jP41ZL9c8PDHIyh8bwRLtTcm1D9SZImlJnt1ir/md2cXjbDaJWFBM5" "\n"\
        "JDGFoqgCWjBH4d1QB7wCCZAA62RjYJsWvIjJEubSfZGL+T0yjWW06XyxV3bqxbYo" "\n"\
        "Ob8VZRzI9neWagqNdwvYkQsEjgfbKbYK7p2CNTUQ" "\n"\
        "-----END CERTIFICATE-----" "\n"\
        "-----BEGIN CERTIFICATE-----" "\n"\
        "MIIF5zCCA8+gAwIBAgIUH32D9G0XU9b+rGiwJq70KUcHRLcwDQYJKoZIhvcNAQEL" "\n"\
        "BQAwezELMAkGA1UEBhMCSU4xFDASBgNVBAgMC01haGFyYXNodHJhMQ0wCwYDVQQH" "\n"\
        "DARQdW5lMRQwEgYDVQQKDAtEaGkgcHZ0IGx0ZDEXMBUGA1UECwwORGhpIHB2dCBs" "\n"\
        "dGQgQ0ExGDAWBgNVBAMMD0RoaSBwdnQgUm9vdCBDQTAeFw0yMTA5MTcxMTIyNTJa" "\n"\
        "Fw0yMTEwMTcxMTIyNTJaMHsxCzAJBgNVBAYTAklOMRQwEgYDVQQIDAtNYWhhcmFz" "\n"\
        "aHRyYTENMAsGA1UEBwwEUHVuZTEUMBIGA1UECgwLRGhpIHB2dCBsdGQxFzAVBgNV" "\n"\
        "BAsMDkRoaSBwdnQgbHRkIENBMRgwFgYDVQQDDA9EaGkgcHZ0IFJvb3QgQ0EwggIi" "\n"\
        "MA0GCSqGSIb3DQEBAQUAA4ICDwAwggIKAoICAQDAe2cs3HXbYDzvrfjNHpF3lfxO" "\n"\
        "Jm/681SB5ScKQR91xZEFyRjPdirvwOm3IX1PfV8/mnnMVwH8TOYLDhBD6FCG3sBT" "\n"\
        "l9iZFHDUBCtKUTRpNFd4TBOSBXomSsjftp2hkezHLdXnLXfdQPMF7YVTYg/LmNbG" "\n"\
        "zjFxix8/uBaoYoypvO79bPn2+UdRIjB/P03hwV5Ano7JaK3x6DztXLXMG5Ytsmbq" "\n"\
        "cbc9rIdeEDBqx6YlHIv6oUmF2+ft4Xr9ToEvRHXGcW5CpEK1dq1+iwV/g21XW/Sq" "\n"\
        "bgiuBJFqIQnUuX8pTNjH4i2KAsffFyh/lOy7U6D/P++/wynB+UK8BlQSoJRznOYG" "\n"\
        "wnMHW9nC05JoWxKwZX7rRash53TqBTfQQHqvv1QM9e/9mou15J6+kxm9u5dWkMkn" "\n"\
        "wVQkHeJPQnM2MtLSR/nre7kZkt7vN6EXce3prToy6v9dCblWbiq+mCEK/e9+MCa8" "\n"\
        "XlhgEMPMe6V3J0IjN6QFZCZi0G9JKaFsgA7oJgBoYhlfRtAgSPlwyYFuz+Etxd6X" "\n"\
        "j6JakZtESHNM8IqjCP6lCgfm/szHbbrLoJrZgotnD0GrjuA69jBmAH6EGwUJrPt2" "\n"\
        "D4ZzMgWsssz+g1pT3dVsWzFyOKM09FtNnQJWMhOAjqqYyC1TnjqRnkci9j/u4I3F" "\n"\
        "1RqpPmZ2Ol2vwZmp0wIDAQABo2MwYTAdBgNVHQ4EFgQUgr9vvSKpMcsQrseiNDtz" "\n"\
        "1kFsaUgwHwYDVR0jBBgwFoAUgr9vvSKpMcsQrseiNDtz1kFsaUgwDwYDVR0TAQH/" "\n"\
        "BAUwAwEB/zAOBgNVHQ8BAf8EBAMCAYYwDQYJKoZIhvcNAQELBQADggIBAGHgyE/X" "\n"\
        "9/+ztprTke6icjke0t+bZt+OrofFFv7sjV7dlxDc/a1smSjO+FRVO7eA1ZfRODJR" "\n"\
        "UFjGb4b76i9PTX+Hz+cGtTKd+B6EIhPmepeQYDdxB/0nOyd+4mzCajiqX55rhhxW" "\n"\
        "uln5IRpEyxhvKGPGHidqCI4vS6671NyfLkL6eZaf4mJpZg6YA5U29yPMm8SMxEl7" "\n"\
        "k7FCPgcFWVUvcFnASbr1d4rlU6B3JCl8c5ge9oBlKboJmqrVFBKZi0uw7zsVFpaK" "\n"\
        "715dag4qaQefQp9Pig9IVLMIGvt440NMzOm5xzXNfXvUHhbe21yasGWJTMB+oUlU" "\n"\
        "0DGCx+YnPgTkPLNYLDUs0TD3/FlrHCqemLZWEccfjCx2HsxxxHlb7aNodmtmaPxs" "\n"\
        "IrXPJ5BnGAmhjPtEYrRCx/OfsGwuKQhERvuhZkqHcdzdCN7sMsP0IhH5uL8UZvhh" "\n"\
        "3lFgqC0mNIrJ4P2XJYmHIFuDU7eRgfe4HlvgYA655867RirG75uIN5rTWTrOjYml" "\n"\
        "3aq15HooHWNM7dMRL1hE1pCm0ECmfmQY30xGgEGsQnoB7xYIdC4E20kxiQ+bNhPW" "\n"\
        "5eRMsZrMYFL//ulHEyRKTaE2X5nYec0wbcsTJG7fhZFcrmtec8MaDsCYNjsmXitb" "\n"\
        "S6hlB0ZS0k/+d+2hmjIOGPWIFFf0mv/dOUZ7" "\n"\
        "-----END CERTIFICATE-----");
}
