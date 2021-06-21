#include <ArduinoWebsockets.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Ticker.h>

#include "led.h"
#include "interface.h"

// 临时定义死，以后换成spiffs或者nvm存储
const char *ssid = "lst";         //Enter SSID
const char *password = "1234567890"; //Enter Password

using namespace websockets;
WebsocketsServer server;       // websocket服务端

void setup()
{
    Serial.begin(115200);
    init_led();
    // Connect to wifi
    WiFi.begin(ssid, password);

    // Wait some time to connect to wifi
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    set_led_state(CONNECTED_WIFI);
    start_broadcast_ip();       // 开始广播本地IP

    // 用于打印ip信息，临时
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP()); //You can get IP address assigned to ESP

    server.listen(8266);
}

// ws消息处理
void onMessageCallback(WebsocketsMessage message) {
    //Serial.print("Got Message: ");
    //Serial.println(message.data());
    Serial.write(message.c_str());      //打印原始字符串到串口
}

// ws事件处理
void onEventsCallback(WebsocketsEvent event, String data) {
    if(event == WebsocketsEvent::ConnectionOpened) {
        Serial.println("Connnection Opened");
    } else if(event == WebsocketsEvent::ConnectionClosed) {
        Serial.println("Connnection Closed");
    } else if(event == WebsocketsEvent::GotPing) {
       // Serial.println("Got a Ping!");
    } else if(event == WebsocketsEvent::GotPong) {
       // Serial.println("Got a Pong!");
    }
}

void loop()
{
    auto client = server.accept();
    client.onMessage(onMessageCallback);
    client.onEvent(onEventsCallback);
    while (client.available()) {
        //uart to ws
        while (Serial.available() > 0) {
            int n = Serial.available();
            char *data = new char[n];
            int len = Serial.readBytes(data, n);
            //push UART data to all connected telnet clients
            client.send(data, len);
            delete[] data;
        }

        // ws to uart on callback
        client.poll();
    }

    // close the connection
    client.close();
    delay(10);
}
