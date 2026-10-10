// Windows implementation of WmataClient::httpGet, using WinHTTP.
// Used for local desktop testing only (excluded from the ESP32 build).

#include "api/WmataClient.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h> // Core Windows types & functions
#include <winhttp.h> // Makes HTTP requests to WMATA API

#include <memory>
#include <stdexcept>
#include <string>

namespace wmata {

    namespace {
        // Closes WinHTTP handles automatically, even if an exception is thrown.
        struct HandleCloser {
            void operator()(HINTERNET h) const { WinHttpCloseHandle(h); }
        };
        using Handle = std::unique_ptr<void, HandleCloser>;

        // Throws if a WinHTTP call failed.
        void check(bool ok, const char* what) {
            if (!ok) {
                throw std::runtime_error(std::string(what) + " failed (Windows error " +
                                        std::to_string(GetLastError()) + ")");
            }
        }

        // Plain ASCII widening. Fine for API keys and station codes.
        std::wstring widen(const std::string& s) { return std::wstring(s.begin(), s.end()); }
    }  // namespace

    //#region PRIVATE Methods declared in api/WmataClient.h
    std::string WmataClient::httpGet(const std::string& path) const {
        Handle session(WinHttpOpen(L"WMATABoard/0.1", WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0));
        check(session != nullptr, "WinHttpOpen");

        Handle connection(WinHttpConnect(session.get(), L"api.wmata.com", INTERNET_DEFAULT_HTTPS_PORT, 0));
        check(connection != nullptr, "WinHttpConnect");

        Handle request(WinHttpOpenRequest(connection.get(), L"GET", widen(path).c_str(), nullptr, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE));
        check(request != nullptr, "WinHttpOpenRequest");

        // WMATA expects the API key as an HTTP header.
        std::wstring headers = L"api_key: " + widen(apiKey_) + L"\r\n";
        check(WinHttpSendRequest(request.get(), headers.c_str(), static_cast<DWORD>(-1), WINHTTP_NO_REQUEST_DATA, 0, 0, 0), "WinHttpSendRequest");
        check(WinHttpReceiveResponse(request.get(), nullptr), "WinHttpReceiveResponse");

        DWORD status = 0;
        DWORD size = sizeof(status);
        check(WinHttpQueryHeaders(request.get(), WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER, WINHTTP_HEADER_NAME_BY_INDEX, &status, &size, WINHTTP_NO_HEADER_INDEX),
            "WinHttpQueryHeaders");
        if (status != 200) {
            throw std::runtime_error("WMATA API returned HTTP " + std::to_string(status));
        }

        // Read the body in chunks until the server has no more data.
        std::string body;
        char buffer[4096];
        DWORD read = 0;
        while (WinHttpReadData(request.get(), buffer, sizeof(buffer), &read) && read > 0) {
            body.append(buffer, read);
        }
        return body;
    }
    //#endregion
}