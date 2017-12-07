#ifndef __GAMSONG_WIFI_H__
#define __GAMSONG_WIIF_H__

extern int WIFI_STAGE;

int    concat(const char *str, int index);
void   sendstring(void);
void   Wifi_connect (const char *SSID, const char *PW);
void   TCPIP_connect(const char *IP, const char *PORT);
void   Request_To_Server(const char *GET, const char *HOST);
void   Request_From_Server(void);
void   Wifi_init(void);
void   WIFI_INIT_STAGE(void);


#endif
