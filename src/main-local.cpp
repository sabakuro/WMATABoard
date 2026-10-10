// This file is for testing - running/retrieving WMATA api data locally
// Polls WMATA for one station and prints the result.
#include <chrono>
#include <exception>
#include <iostream>
#include <thread>

#include "api/WmataClient.h"
#include "config/config.h"
#include "config/secrets.h"
#include "models/StationInfo.h"

namespace {
    // Prints out information to display on LED Board, takes StationInfo object
    void printBoard(const wmata::StationInfo& data) {
        if (data.empty()) {
            std::cout << "No predictions available right now.\n";
            return;
        }

        std::cout << "Next arrivals for " << data.stationName << ":\n";

        for (const auto& t : data.trains) {
            std::cout << "LN: " << t.line << " | DEST: " << t.destinationName << " | MIN: " << t.minutesRaw << '\n';
        }
    }
}

int main() {
    wmata::WmataClient client(secrets::WMATA_API_KEY);

    // Infinite loop for printing Station Board - equivalent is void loop() in the Arduino platform
    // Sleep timing each loop based on REFRESH_SECONDS config value
    for (;;) {
        try {
            printBoard(client.fetchStation(config::STATION_CODE, config::STATION_NAME));
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << '\n';
        }
        std::cout << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(config::REFRESH_SECONDS));
    }
}