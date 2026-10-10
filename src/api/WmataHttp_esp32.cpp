// ESP32 implementation of WmataClient::httpGet, using Arduino's HTTPClient.
// Excluded from the desktop (CMake) build.

#include "api/WmataClient.h"

#include <Arduino.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

#include <stdexcept>
#include <string>

namespace wmata
{

    // #region PRIVATE Methods declared in api/WmataClient.h
    std::string WmataClient::httpGet(const std::string &path) const
    {
        Serial.println("httpGet called");
        WiFiClientSecure client;
        client.setInsecure(); // Skips certificate validation; pin WMATA's root CA for production use.

        HTTPClient http;
        http.setTimeout(10000);
        if (!http.begin(client, String("https://api.wmata.com") + path.c_str()))
        {
            throw std::runtime_error("HTTP begin failed");
        }

        // WMATA expects the API key as an HTTP header.
        http.addHeader("api_key", apiKey_.c_str());

        int code = http.GET();
        if (code != HTTP_CODE_OK)
        {
            std::string message = (code > 0)
                                      ? "WMATA API returned HTTP " + std::to_string(code)
                                      : std::string("Connection failed: ") + HTTPClient::errorToString(code).c_str();
            http.end();
            throw std::runtime_error(message);
        }

        std::string body(http.getString().c_str());
        http.end();
        return body;
    }
    // #endregion
}