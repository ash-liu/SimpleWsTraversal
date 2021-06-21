#ifndef _LED_H
#define _LED_H

// 用于指示设备状态
enum {
    INIT,
    CONNECTED_WIFI,
    CLIENT_ACCEPTED,
    DATA_INOUT,
    CLIENT_CLOSED,
};

#define LED         2           // GPIO2 for esp-01s
#define ON          0
#define OFF         1


void init_led(void);
void set_led_state(int s);

#endif