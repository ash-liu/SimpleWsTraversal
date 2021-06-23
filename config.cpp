#include <ArduinoJson.h>
#include "FS.h"
#include <LittleFS.h>
#include "config.h"

bool loadConfig(struct wifi_info *info)
{
    if(!LittleFS.begin()){
        Serial.println("An Error has occurred while mounting LittleFS");
        return false;
    }
    
    File configFile = LittleFS.open("/config.json", "r");
    if (!configFile)
    {
        Serial.println("Failed to open config file");
        return false;
    }

    size_t size = configFile.size();
    if (size > MAX_CONFIG_LENGTH)
    {
        Serial.println("Config file size is too large");
        return false;
    }

    // Allocate a buffer to store contents of the file.
    std::unique_ptr<char[]> buf(new char[size]);

    // We don't use String here because ArduinoJson library requires the input
    // buffer to be mutable. If you don't use ArduinoJson, you may as well
    // use configFile.readString instead.
    configFile.readBytes(buf.get(), size);

    StaticJsonDocument<MAX_CONFIG_LENGTH> doc;
    auto error = deserializeJson(doc, buf.get());
    if (error)
    {
        Serial.println("Failed to parse config file");
        return false;
    }

    const char *ssid = doc["ssid"];
    const char *passwd = doc["passwd"];

    strcpy(info->ssid, ssid);
    strcpy(info->passwd, passwd);
    // Real world application would store these values in some variables for
    // later use.

    Serial.print("SSID: ");
    Serial.println(ssid);
    Serial.print("PASSWD: ");
    Serial.println(passwd);
    return true;
}

bool saveConfig(struct wifi_info *info)
{
    StaticJsonDocument<MAX_CONFIG_LENGTH> doc;
    doc["ssid"] = info->ssid;
    doc["passwd"] = info->passwd;

    if(!LittleFS.begin()){
        Serial.println("An Error has occurred while mounting LittleFS");
        return false;
    }

    File configFile = LittleFS.open("/config.json", "w");
    if (!configFile)
    {
        Serial.println("Failed to open config file for writing");
        return false;
    }

    serializeJson(doc, configFile);
    return true;
}

// 注意，这里由于与LED复用了pin，所以在按键释放前阻塞
bool check_need_smartconfig_blocking()
{
    int cnt=0;
    pinMode(SMARTCONFIG_BUTTON, INPUT_PULLUP);

    while (cnt++ <= WAIT_SMARTCONFIG_BUTTON_TIME * 10) {
        if (digitalRead(SMARTCONFIG_BUTTON) == 0) {
            while(digitalRead(SMARTCONFIG_BUTTON) == 0) {
                // 等待直到按键释放
            }
            return true;
        }
        else {
            delay(100);
        }
    }
    return false;
}