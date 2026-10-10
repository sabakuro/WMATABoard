#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include <Fonts/TomThumb.h>
#include <string>

class Display
{
public:
    Display(MatrixPanel_I2S_DMA &panel);
    void init();
    void printHeader();
    void printRow(int line, std::string dest, std::string time);
    void clear();

private:
    MatrixPanel_I2S_DMA &panel;
    int rowHeight = 5;
    int rowSpacing = 8;
    int boardWidth = 64;
    void rightAlign(std::string s, int y);
};