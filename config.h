#ifndef _CONFIG_H
#define _CONFIG_H

#define MAX_WIFI_INFO_LENGTH    50
#define MAX_CONFIG_LENGTH   1024

#define SMARTCONFIG_BUTTON  2
#define WAIT_SMARTCONFIG_BUTTON_TIME    3       // s

struct wifi_info
{
    char ssid[MAX_WIFI_INFO_LENGTH];
    char passwd[MAX_WIFI_INFO_LENGTH];
};


bool loadConfig(struct wifi_info *info);
bool saveConfig(struct wifi_info *info);
bool check_need_smartconfig_blocking();

#endif
