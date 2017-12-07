#include <stm32f10x_usart.h>
#include <stdlib.h>
#include <string.h>

#include "Gamsong_led.h"
#include "Gamsong_usart.h"
#include "Gamsong_json.h"
#include "Gamsong_algorithm.h"

char sky[10]             = "";
char temp_current[10]    = "";
char temp_max[10]        = "";
char temp_min[10]         = "";
char is_rain_snow[10]     = "";
char rain_snow_1hour[10]  = ""; 
char rain_1day[10]        = "";
char rain_last_10min[10]  = "";
char rain_last_15min[10]  = "";
char rain_last_30min[10]  = "";
char rain_last_1hour[10]  = "";
char time_current[30]     = "";


void substring(char* input, int start){
	int i = 0;
	while(JSON[start] != '"')
		input[i++] = JSON[start++];
	input[i] = '\0';
}

void getValue(char* input, char* find) {
	int f_SZ = strlen(find), s_SZ = JSON_SZ;
    int f_iter = 0, s_iter = 0;
	int value_ptr = -1;
    for(; s_iter < s_SZ && value_ptr == -1 ; ++s_iter){
        if(find[f_iter] == JSON[s_iter])  f_iter++;
		else                              f_iter = 0;
        if(f_SZ == f_iter)
			value_ptr = s_iter;
    }
	if(value_ptr != -1)
		substring(input,value_ptr+4);

}



void json_parsing(){
	getValue(sky,"code");
	getValue(temp_current,"tc");
	getValue(temp_max,"tmax");
	getValue(temp_min,"tmin");
	getValue(is_rain_snow,"type");
	getValue(rain_snow_1hour,"sinceOntime");
	getValue(rain_1day,"sinceMidnight");
	getValue(rain_last_10min,"last10min");
	getValue(rain_last_15min,"last15min");
	getValue(rain_last_30min,"last30min");
	getValue(rain_last_1hour,"last1hour");
	getValue(time_current,"timeObservation");


	USART_Puts(USART1, sky);
	USART_Puts(USART1, "\r\n");
	USART_Puts(USART1, temp_current);
	USART_Puts(USART1, "\r\n");
	USART_Puts(USART1, temp_max);
	USART_Puts(USART1, "\r\n");
	USART_Puts(USART1, temp_min);
	USART_Puts(USART1, "\r\n");
	USART_Puts(USART1, is_rain_snow);
	USART_Puts(USART1, "\r\n");
	USART_Puts(USART1, rain_snow_1hour);
	USART_Puts(USART1, "\r\n");
	USART_Puts(USART1, rain_1day);
	USART_Puts(USART1, "\r\n");
	USART_Puts(USART1, rain_last_10min);
	USART_Puts(USART1, "\r\n");
	USART_Puts(USART1, rain_last_15min);
	USART_Puts(USART1, "\r\n");
	USART_Puts(USART1, rain_last_30min);
	USART_Puts(USART1, "\r\n");
	USART_Puts(USART1, rain_last_1hour);
	USART_Puts(USART1, "\r\n");
	USART_Puts(USART1, time_current);
	USART_Puts(USART1, "\r\n");
}
