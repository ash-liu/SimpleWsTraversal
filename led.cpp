#include <Arduino.h>
#include "led.h"

void init_led()
{
    pinMode(LED, OUTPUT);
}

void set_led_state(int s)
{
    switch (s) {
        case INIT:
            digitalWrite(LED, OFF);
            break;
        case CONNECTED_WIFI:
            digitalWrite(LED, ON);
            break;
        default:
            digitalWrite(LED, ON);
    }
}