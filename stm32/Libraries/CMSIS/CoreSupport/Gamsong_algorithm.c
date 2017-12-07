#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "Gamsong_algorithm.h"
#include "Gamsong_usart.h"
#include "Gamsong_json.h"
#include "Gamsong_adc.h"

char GAMSONG_STRING[20] = "";
char weather[10] = "";

int parseInt(char* str, int len)
{
    int mult = 1, re = 0, i = len-1;
    for(; i >= 0 ; i--){
        re += ((int)str[i] -48)*mult;
        mult = mult*10;
    }
    return re;
}

void Delay(uint32_t us){
	if(us>1){
		uint32_t count=us*8-6;
		while(count--);
	}else{
		uint32_t count=2;
		while(count--);
	}
}


int compareStr(char* lhs, char* rhs){
    int lhs_size = strlen(lhs);
    int rhs_size = strlen(rhs);
    int iter_max = lhs_size < rhs_size ? lhs_size : rhs_size;
    int iter = 0;

    for(iter = 0; iter < iter_max ; ++iter)
        if(lhs[iter] != rhs[iter])
            return 0;
    return 1;
}

void replaceStr(char* str, const char* replace){
    int SZ = strlen(replace);
    int i = 0;
    for(i = 0; i < SZ ; ++i)
        str[i] = replace[i];
}

int findStr(const char* find, char* search){
    int f_SZ = strlen(find), s_SZ = strlen(search);
    int f_iter = 0, s_iter = 0;
    for(; s_iter < s_SZ ; ++s_iter){
        if(find[f_iter] == search[s_iter])  f_iter++;
        else                                f_iter = 0;
        if(f_SZ == f_iter)  return 1;
    }

    return 0;
}

void jsonCopy(char* str, int SZ){
    int i;
    for(i = 0; i < SZ ; ++i)
    	JSON[JSON_SZ++] = str[i];
}

char* isWeather(char *s){
    char str[2];
    int weatherNum;

    strncpy(str, s+5,2);
    weatherNum = parseInt(s,2);
    if(weatherNum != 2 && (weatherNum % 2 == 0))
        replaceStr(weather, "rainy");
    else if(weatherNum != 1 && weatherNum % 4 == 1)
        replaceStr(weather, "snowy");
    else
        replaceStr(weather, "clear");
    return weather;
}

int parse_year(char* time){
    char year[4];
    strncpy(year, time, 4);
    return parseInt(year,4);
}
int parse_month(char* time){
    char month[2];
    strncpy(month, time+5, 2);
    return parseInt(month,2);
}
int parse_day(char* time){
    char day[2];
    strncpy(day, time+8, 2);
    return parseInt(day,2);
}
int parse_hour(char* time){
    char hour[2];
    strncpy(hour, time+11,2);
    return parseInt(hour,2);
}
int parse_min(char* time){
    char min[2];
    strncpy(min, time+14,2);
    return parseInt(min,2);
}
int parse_sec(char* time){
    char s[2];
    strncpy(s, time+17,2);
    return parseInt(s,2);
}


void set_GamsongString(){
    int now_temp = parseInt(temp_current, strlen(temp_current));
    if(sensor_rain <= 2000 && compareStr(isWeather(sky),"rainy") && (parseInt(type,strlen(type)) == 1 || parseInt(type,1) == 2))
        replaceStr(GAMSONG_STRING,"senti");
    else if(compareStr(isWeather(sky), "snowy") && parseInt(type,strlen(type)) == 3)
        replaceStr(GAMSONG_STRING,"comfortable");
    else{
        if(now_temp <= 10){
            if(sensor_temper <= 20)
                replaceStr(GAMSONG_STRING,"sad");
            else
                replaceStr(GAMSONG_STRING,"comfortable");
        }
        else if(now_temp >= 28){
            if(sensor_temper <= 23)
                replaceStr(GAMSONG_STRING, "happy");
            else
                replaceStr(GAMSONG_STRING, "depressed");
        }
        else{
            if(parse_month(time_current) < 7)
                replaceStr(GAMSONG_STRING, "ssum");
            else if(parse_month(time_current) > 9)
                replaceStr(GAMSONG_STRING, "depressed");
        }
    }
}
