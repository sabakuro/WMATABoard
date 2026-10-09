#include <Arduino.h>

// put function declarations here:
int myFunction(int, int);

void setup()
{
  Serial.begin(115200);
  delay(1000);
  Serial.println("Hello world!");
}

void loop()
{
  Serial.println("esp32 is running!");
  delay(1000);
}
