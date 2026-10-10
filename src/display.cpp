#include "display.h"
#include <Fonts/TomThumb.h>
#include <string>

Display::Display(MatrixPanel_I2S_DMA &panel)
    : panel(panel) {}

void Display::init()
{
    panel.setBrightness8(128); // Low brightness, range 0–255
    panel.clearScreen();
    panel.setFont(&TomThumb);
    panel.setTextSize(1);
    panel.setTextWrap(false);
}

void Display::printHeader()
{
    panel.setTextColor(panel.color565(255, 0, 0));
    panel.setCursor(0, rowHeight);
    panel.print("LN   DEST");
    std::string minString = "MIN";
    rightAlign(minString, rowHeight);
    panel.print(minString.c_str());
}

void Display::printRow(int line, std::string dest, std::string time)
{
    int rowY = rowSpacing * line;
    panel.fillRect(0, rowY, 3, rowHeight, panel.color565(128, 128, 128));
    panel.setTextColor(panel.color565(255, 174, 66));
    panel.setCursor(10, rowY + rowHeight);
    panel.print(dest.c_str());
    rightAlign(time, rowY + rowHeight);
    panel.print(time.c_str());
}

void Display::rightAlign(std::string s, int y)
{
    int16_t x1, y1;
    uint16_t w, h;
    panel.getTextBounds(s.c_str(), 0, 0, &x1, &y1, &w, &h);
    panel.setCursor(boardWidth - w, y);
}

void Display::clear()
{
    panel.clearScreen();
}