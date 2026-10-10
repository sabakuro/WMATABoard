#pragma once

// Settings that determine which Metro Station to retrieve WMATA API data from, and how often to refresh

namespace config
{
    // inline constexpr: guarantees a single CONSTANT global instance of the variable,
    // evaluated at compile-time, with no multiple-definition errors.
    inline constexpr const char *STATION_CODE = "N01";    // e.g. N01 for McLean
    inline constexpr const char *STATION_NAME = "McLean"; // e.g. McLean
    inline constexpr int REFRESH_SECONDS = 30;
}