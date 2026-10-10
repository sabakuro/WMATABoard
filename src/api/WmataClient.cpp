#include "api/WmataClient.h"

#include <stdexcept>
#include <string>
#include <utility>

#include <nlohmann/json.hpp> // Third party JSON library (CMake on desktop, PlatformIO lib_deps on ESP32)

// Platform-independent half of the client: turns WMATA's JSON into StationInfo objects.
// The platform-specific httpGet() lives in WmataHttp_win.cpp / WmataHttp_esp32.cpp.

namespace wmata {

    namespace {
        using json = nlohmann::json;

        // Reads a string field. Returns "" if the field is missing OR null
        // (WMATA sends null for some fields, which would make json::value() throw).
        std::string str(const json& obj, const char* key) {
            auto it = obj.find(key);
            return (it != obj.end() && it->is_string()) ? it->get<std::string>() : std::string{};
        }

        // "5" -> 5, "ARR"/"BRD" -> 0, anything else ("---") -> -1.
        int parseMinutes(const std::string& raw) {
            if (raw == "ARR" || raw == "BRD") return 0;
            bool digits = !raw.empty() && raw.find_first_not_of("0123456789") == std::string::npos;
            return digits ? std::stoi(raw) : -1;
        }
    }  // namespace

    //#region PUBLIC Methods
    WmataClient::WmataClient(std::string apiKey) : apiKey_(std::move(apiKey)) {}

    StationInfo WmataClient::fetchStation(const std::string& stationCode, const std::string& stationName) const {
        const std::string body = httpGet("/StationPrediction.svc/json/GetPrediction/" + stationCode);

        json doc = json::parse(body, nullptr, /*allow_exceptions=*/false);
        if (doc.is_discarded()) throw std::runtime_error("Could not parse WMATA response");

        StationInfo board;
        board.stationCode = stationCode;
        board.stationName = stationName;
        board.fetchedAt = std::chrono::system_clock::now();

        auto trains = doc.find("Trains");
        if (trains == doc.end() || !trains->is_array()) return board;

        // Loops through retrieved JSON data for Train information
        for (const auto& item : *trains) {
            Train t;
            t.line = str(item, "Line");
            t.destination = str(item, "Destination");
            t.destinationName = str(item, "DestinationName");
            t.destinationCode = str(item, "DestinationCode");
            t.group = str(item, "Group");
            t.car = str(item, "Car");
            t.minutesRaw = str(item, "Min");
            t.minutes = parseMinutes(t.minutesRaw);
            board.trains.push_back(std::move(t));
        }
        return board;
    }
    //#endregion
}