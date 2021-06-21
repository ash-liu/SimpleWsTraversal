#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Ticker.h>
#include "interface.h"


// struct interface interface;

Ticker t_udp;
WiFiUDP udp;

void broadcast_ip()
{
    char buffer[BROADCAST_LENGTH] = "8";

    if (WiFi.status() != WL_CONNECTED) {
        t_udp.detach();
        return;
    }
    udp.beginPacket("255.255.255.255", 8266);
    sprintf(buffer, "ip:%s\n", WiFi.localIP().toString().c_str());
    udp.write(buffer, strlen(buffer));
    udp.endPacket();
    // Serial.println(WiFi.localIP());
    // Serial.printf("Now listening at IP %s\n", WiFi.localIP().toString().c_str());
    // Serial.printf("buff: %s\n", buffer);
}


void start_broadcast_ip()
{
    udp.begin(8265);
    t_udp.attach(1, broadcast_ip);
}