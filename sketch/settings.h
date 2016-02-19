#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdint.h>

extern const char* ssid;
extern const char* password;
extern const char* devicename; // your device will be discoverable at http://devicename.local
extern const int iLanguage;
extern const uint16_t serverHttpPort; // not sure if it makes sense to have that here


#endif // SETTINGS_H
