#include <ArduinoWebsockets.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Ticker.h>

#include "led.h"
#include "interface.h"
#include "config.h"

using namespace websockets;
WebsocketsServer server; // websocket服务端

struct wifi_info wifi_config; // wifi配置信息

void setup()
{
    int ret;
    bool force_smartconfig;

    // 1. 初始化串口
    Serial.begin(115200);

    // 2. 检查是否强制smartconfig
    force_smartconfig = check_need_smartconfig_blocking();

    // 3. 初始化LED
    init_led();

    // 4. 获取wifi配置
    ret = loadConfig(&wifi_config);
    if (!ret || force_smartconfig) {
        Serial.print("Start smart config.");
        set_led_state(SMART_CONFIG);
        WiFi.mode(WIFI_STA);
        WiFi.beginSmartConfig();
        // TODO: set led

        // 等待smartconfig配置完成
        int wait_times = 120;
        while (1) {
            if (WiFi.smartConfigDone()) {
                strcpy(wifi_config.ssid, WiFi.SSID().c_str());
                strcpy(wifi_config.passwd, WiFi.psk().c_str());
                saveConfig(&wifi_config);
                break;
            }
            else {
                if (wait_times == 0) {
                    // 配置不成功重启
                    delay(500);
                    ESP.restart();
                }

                delay(1000);
                wait_times--;
            }
        }
    }

    // 4. 连接wifi
    set_led_state(BEGIN_CONNECT_WIFI);
    WiFi.begin(wifi_config.ssid, wifi_config.passwd);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    set_led_state(CONNECTED_WIFI);

    // 5. 开始广播本地IP
    start_broadcast_ip(); // 开始广播本地IP

    // 6. 用于打印ip信息，临时
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP()); //You can get IP address assigned to ESP

    // 6. 开启ws server
    server.listen(8266);
}

// ws消息处理
void onMessageCallback(WebsocketsMessage message)
{
    //Serial.print("Got Message: ");
    //Serial.println(message.data());
    Serial.write(message.c_str()); //打印原始字符串到串口
}

// ws事件处理
void onEventsCallback(WebsocketsEvent event, String data)
{
    if (event == WebsocketsEvent::ConnectionOpened)
    {
        Serial.println("Connnection Opened");
    }
    else if (event == WebsocketsEvent::ConnectionClosed)
    {
        Serial.println("Connnection Closed");
    }
    else if (event == WebsocketsEvent::GotPing)
    {
        // Serial.println("Got a Ping!");
    }
    else if (event == WebsocketsEvent::GotPong)
    {
        // Serial.println("Got a Pong!");
    }
}

void loop()
{
    auto client = server.accept();
    client.onMessage(onMessageCallback);
    client.onEvent(onEventsCallback);
    while (client.available())
    {
        //uart to ws
        while (Serial.available() > 0)
        {
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
