#pragma once
#include <string>

namespace wmata {
    // Represents an incoming train, as reported by WMATA's predictions API.
    // Plain data only (no API or display code) so it can be passed anywhere
    struct Train {
        std::string line;             // Metro Line code, "SV"
        std::string destination;      // Short name, e.g. "Ashburn"
        std::string destinationName;  // Full name, e.g. "Ashburn"
        std::string destinationCode;  // Station code, e.g. "N12"
        std::string group;            // Track group, "1" or "2"
        std::string car;              // Number of cars: "6", "8", or "-"
        std::string minutesRaw;       // Raw API value: "3", "ARR", "BRD", or "---"

        // Minutes until arrival. 0 for ARR/BRD, -1 if unknown ("---").
        int minutes = -1;

        bool isArriving() const { return minutesRaw == "ARR"; }
        bool isBoarding() const { return minutesRaw == "BRD"; }
        bool hasEstimate() const { return minutes >= 0; }

        std::string displayMinutes() const {
            if (isArriving()) return "ARR";
            if (isBoarding()) return "BRD";
            if (hasEstimate()) return std::to_string(minutes);
            return "--";
        }
    };
}