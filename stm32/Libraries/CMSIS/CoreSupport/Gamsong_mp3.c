#include <stm32f10x_usart.h>
#include <stdint.h>

#include "Gamsong_mp3.h"
#include "Gamsong_usart.h"
#include "Gamsong_led.h"
#include "Gamsong_algorithm.h"



/****
 1: 6개
 2: 5개
 3: 3개
 4: 3개
 5: 5개
 6: 3개
 ******/
int mp3_start[7] = {0,1,7,12,15,18,23};
int mp3_SZ[7] = {0,6,5,3,3,5,3};
int folder = 1, track = 1;

char send_buf[10] = {0x7E, 0xFF, 06, 00, 00, 00, 00, 00, 00, 0xEF};
uint16_t is_reply = 0;

//
void mp3_set_reply (uint16_t state) {
	is_reply = state;
	send_buf[4] = is_reply;
}

//
void fill_uint16_bigend (char *thebuf, uint16_t data) {
	*thebuf 	=	0xFF & (data>>8);
	*(thebuf+1) =	0xFF & data;
}


//calc checksum (1~6 byte)
uint16_t mp3_get_checksum (char *thebuf) {
	uint16_t sum = 0;
	int i;
	for (i=1; i<7; i++) {
		sum += thebuf[i];
	}
	return -sum;
}

//fill checksum to send_buf (7~8 byte)
void mp3_fill_checksum () {
	uint16_t checksum = mp3_get_checksum (send_buf);
	fill_uint16_bigend (send_buf+7, checksum);
}


void s_send_func () {
	int i=0;
	LED_TurnOn(1,1,0,0);
	for(i = 0 ; i < 10 ; ++i){
		USART_SendData(USART3,send_buf[i]);
		while(!(USART3->SR & USART_SR_TXE)){}
		USART_SendData(USART1,send_buf[i]);
		while(!(USART1->SR & USART_SR_TXE)){}
	}
	Delay(30000);
	LED_TurnOff(1,1,0,0);

}

void mp3_send_cmd_num (char cmd, uint16_t arg) {
	send_buf[3] = cmd;
	fill_uint16_bigend ((send_buf+5), arg);
	mp3_fill_checksum ();
	s_send_func ();
}


void mp3_send_cmd (char cmd) {
	send_buf[3] = cmd;
	fill_uint16_bigend ((send_buf+5), 0);
	mp3_fill_checksum ();
	s_send_func ();
}



void mp3_play_physical_num (uint16_t num) {
	mp3_send_cmd_num (0x03, num);
}

void mp3_play_physical () {
	mp3_send_cmd (0x03);
}



//0x06 set volume 0-30
void mp3_set_volume (uint16_t volume) {
	mp3_send_cmd_num (0x06, volume);
}

//0x07 set EQ0/1/2/3/4/5    Normal/Pop/Rock/Jazz/Classic/Bass
void mp3_set_EQ (uint16_t eq) {
	mp3_send_cmd_num (0x07, eq);
}

//0x09 set device 1/2/3/4/5 U/SD/AUX/SLEEP/FLASH
void mp3_set_device (uint16_t device) {
	mp3_send_cmd_num (0x09, device);
}

void mp3_sleep () {
	mp3_send_cmd (0x0a);
}

void mp3_reset () {
	mp3_send_cmd (0x0c);
}

void mp3_play () {
	mp3_send_cmd (0x0d);
}

void mp3_pause () {
	mp3_send_cmd (0x0e);
}

void mp3_stop () {
	mp3_send_cmd (0x16);
}

void mp3_play_num (uint16_t num) {
	mp3_send_cmd_num (0x12, num);
}

void mp3_get_state () {
	mp3_send_cmd (0x42);
}

void mp3_get_volume () {
	mp3_send_cmd (0x43);
}


void mp3_get_u_sum () {
	mp3_send_cmd (0x47);
}

void mp3_get_tf_sum () {
	mp3_send_cmd (0x48);
}

void mp3_get_flash_sum () {
	mp3_send_cmd (0x49);
}


void mp3_get_tf_current () {
	mp3_send_cmd (0x4c);
}

void mp3_get_u_current () {
	mp3_send_cmd (0x4b);
}


void mp3_get_flash_current () {
	mp3_send_cmd (0x4d);
}

void mp3_single_loop (uint16_t state) {
	mp3_send_cmd_num (0x19, !state);
}

void mp3_single_play (uint16_t num) {
	int i = 100;
	mp3_play_num(num);
	while(i--);
	mp3_single_loop (1);
	//mp3_send_cmd (0x19, !state);
}

void mp3_DAC (uint16_t state) {
	mp3_send_cmd_num (0x1a, !state);
}

void mp3_random_play () {
	mp3_send_cmd (0x18);
}



void mp3_select_folder(){
	folder = get_GamsongNum();

	mp3_play_physical_num((uint16_t)mp3_start[folder]);

}
void mp3_next () {
	if(track < mp3_SZ[folder])
		track++;
	else
		track = 0;

	mp3_play_physical_num((uint16_t)(mp3_start[folder]+track));

}	

void mp3_prev () {
	if(1 < track)
		track--;
	else
		track = mp3_SZ[folder];
	mp3_play_physical_num((uint16_t)(mp3_start[folder]+track));

}
