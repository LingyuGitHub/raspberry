#include <ArduiPi_OLED_lib.h>
#include <ArduiPi_OLED.h>
#include <Adafruit_GFX.h>
#include <stdio.h>

#include "oled.h"

ArduiPi_OLED display;

extern bool oled_init()
{
    if (!display.init(OLED_I2C_RESET, 2))
    {
        return false;
    }
    display.begin();
    display.setTextSize(1.8);
    display.setTextColor(WHITE);
    return true;
}

extern void oled_writetext(const char* str)
{
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print(str);
    display.display();
}

int main(int argc, char *argv[])
{
    int i=0;
    oled_init();

    while (true)
    {
        for (i=0; i<argc; i++)
        {
            oled_writetext(argv[i]);
            sleep(1);
        }
    }
    return 1;
}
