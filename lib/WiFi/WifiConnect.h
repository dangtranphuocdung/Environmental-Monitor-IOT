#ifndef WIFI_CONNECT_H
#define WIFI_CONNECT_H

#include <WiFi.h>


class Wifi {
    public:
        void connect(String wifi_ssid, String wifi_password);
};

#endif
