#ifndef __GAMSONG_JSON_H__
#define __GAMSONG_JSON_H__

extern char sky[10]             ;
extern char temp_current[10]    ;
extern char temp_max[10]        ;
extern char temp_min[10]        ;
extern char is_rain_snow[10]    ;
extern char rain_snow_1hour[10] ;
extern char rain_1day[10]       ;
extern char rain_last_10min[10] ;
extern char rain_last_15min[10] ;
extern char rain_last_30min[10] ;
extern char rain_last_1hour[10] ;
extern char time_current[30]    ;


void substring(char* input, int start);
void getValue(char* input, char* find);

void json_parsing(void);

#endif
