#include <Arduino.h>
#include <WiFi.h>
#include <display.h>

#include <exception>
#include <string>

#include "api/WmataClient.h"
#include "config/config.h"
#include "config/secrets.h"
 
Display *display = nullptr;
 
wmata::WmataClient client(secrets::WMATA_API_KEY);
wmata::StationInfo station; // Store latest successful fetch
 
static const int MAX_ROWS = 3; // Most amount of rows to display on LED Board
 
// Fetches fresh data. On failure the previous data stays on screen.
void refreshTrains()
{
  try
  {
    station = client.fetchStation(config::STATION_CODE, config::STATION_NAME);
  }
  catch (const std::exception &e)
  {
    Serial.printf("Fetch failed: %s\n", e.what());
  }
}

void setup()
{
  Serial.begin(115200);
  delay(1000);

  HUB75_I2S_CFG::i2s_pins pins = {
      25, 27, 26,     // R1, G1, B1
      14, 32, 13,     // R2, G2, B2
      33, 21, 22, 23, // A, B, C, D
      -1,             // E unused
      19, 17, 18      // LAT, OE, CLK
  };

  HUB75_I2S_CFG config(64, 32, 1, pins);
  config.clkphase = false;
  static MatrixPanel_I2S_DMA panel(config);

  if (!panel.begin())
  {
    Serial.println("Display initialization failed");
    return;
  }

  display = new Display(panel);
  display->init();
}

void loop()
{
  if (display != nullptr)
  {
    display->printHeader();
    display->printRow(1, "Ashburn", "BRD");
    display->printRow(2, "Vienna", "6");
    display->printRow(3, "Ashburn", "9");
  }

  delay(30);
}