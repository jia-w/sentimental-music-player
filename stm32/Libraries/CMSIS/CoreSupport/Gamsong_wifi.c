#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stm32f10x_usart.h>
#include "Gamsong_usart.h"
#include "Gamsong_wifi.h"
#include "Gamsong_led.h"
#include "Gamsong_algorithm.h"

char AT_wifi_command[300], int_to_char[10];
int  AT_wifi_command_SZ = 0;
int WIFI_STAGE = 0;
char* msg;



int concat(const char *str, int index){
    while(*str){
        AT_wifi_command[index++] = *str;
        str++;
    }
    return index;
}

void sendstring(){
    int index;
    for(index = 0 ; index < AT_wifi_command_SZ ; ++index){
       USART_SendData(UART4, AT_wifi_command[index]);
       while(!(UART4->SR & USART_SR_TXE)){}
    }
}

void Wifi_connect(const char *SSID, const char *PW){
    int index = 0;
    index = concat("AT+CWJAP=\"",0);
    index = concat(SSID,index);
    index = concat("\",\"",index);
    index = concat(PW,index);
    index = concat("\"",index);
    index = concat("\r\n",index);
    AT_wifi_command_SZ = index;
    sendstring();
}  

void TCPIP_connect(const char *IP, const char *PORT){
    int index = 0;
    index = concat("AT+CIPSTART=\"TCP\",\"",0);
    index = concat(IP,index);
    index = concat("\",",index);
    index = concat(PORT,index);
    index = concat("\r\n",index);
    AT_wifi_command_SZ = index;
    sendstring();
}  

 void Request_To_Server_SIZE(){
     int index = 0;
     index = concat("AT+CIPSEND=181",0);
     index = concat("\r\n", index);
     AT_wifi_command_SZ = index;
     sendstring();
     Delay(100000);
 }

void Request_To_Server(const char *GET, const char *HOST){
    int index = 0;
    index = concat(GET,0);
    index = concat("\r\n",index);
    index = concat(HOST,index);
    index = concat("\r\n\r\n",index);
    AT_wifi_command_SZ = index;
    LED_TurnOn(0,1,1,0);
    LED_TurnOffAll();
    sendstring();
    LED_TurnOn(0,1,1,0);
    LED_TurnOffAll();
}

void Request_From_Server(){
    int index = 0;
    index = concat("+IPD",0);
    index = concat("\r\n",index);
    AT_wifi_command_SZ = index;
    sendstring();
}

void WIFI_INIT_STAGE(){
    switch(WIFI_STAGE){
        case 0:
            //Wifi_init();
            break;
        case 1:
            Wifi_connect("gm", "fxjis6cu3o85e");
            break;
        case 2:
            TCPIP_connect("1.234.67.10","80");
            break;
        case 3:
            Request_To_Server_SIZE();
            Request_To_Server("GET /weather/current/minutely?appKey=09c32811-7343-3537-964e-9c58ab2ea046&lat=35.233315&lon=129.079475&village=&county=&stnid=&city=&version=1 HTTP/1.1","Host: apis.skplanetx.com");
            Delay(100000);
            break;
        default:
            break;
    } 
    Delay(3000);
}



void Wifi_init(void){
    USART_Puts(UART4,"AT+RST\r\n");
    Delay(300);
    USART_Puts(UART4,"AT+CIFSR\r\n");
    Delay(300);
    USART_Puts(UART4,"AT+CWMODE=1\r\n");
    Delay(300);
}
