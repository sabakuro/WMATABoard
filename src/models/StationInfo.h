#pragma once
#include <chrono>
#include <string>
#include <vector>
#include "models/train.h"

namespace wmata {
    // A snapshot of one station's upcoming trains at a moment in time.
    // This is the object you hand to whatever displays the data
    struct StationInfo {
        std::string stationCode;  // e.g. "N06"
        std::string stationName;  // e.g. "McLean"
        std::vector<Train> trains; // Array of Train objects
        std::chrono::system_clock::time_point fetchedAt{};

        bool empty() const { return trains.empty(); }
    };
} 