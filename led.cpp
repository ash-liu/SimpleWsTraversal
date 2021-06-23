#include <Arduino.h>
#include <Ticker.h>
#include "led.h"

Ticker t_led;

void init_led()
{
    pinMode(LED, OUTPUT);
}

void flash_led()
{
    digitalWrite(LED, !digitalRead(LED));
}

void set_led_state(int s)
{
    t_led.detach();
    switch (s) {
        case INIT:
            digitalWrite(LED, OFF);
            break;
        case SMART_CONFIG:
            t_led.attach(1, flash_led);
            break;
        case BEGIN_CONNECT_WIFI:
            t_led.attach(0.1, flash_led);
            break;
        case CONNECTED_WIFI:
            digitalWrite(LED, ON);
            break;
        default:
            digitalWrite(LED, ON);
            break;
    }
}