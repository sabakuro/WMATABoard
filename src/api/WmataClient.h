#pragma once
#include <string>
#include "models/StationInfo.h"

namespace wmata {
    // Talks to WMATA's Real-Time Predictions API over HTTPS (WinHTTP on Windows).
    // Throws std::runtime_error on network, HTTP, or parse failures.
    class WmataClient {
        public:
            explicit WmataClient(std::string apiKey);

            // Fetches the current predictions for one station.
            StationInfo fetchStation(const std::string& stationCode,
                                    const std::string& stationName = "") const;

        private:
            // Performs an HTTPS GET against api.wmata.com and returns the response body.
            std::string httpGet(const std::string& path) const;

            std::string apiKey_;
    };
}