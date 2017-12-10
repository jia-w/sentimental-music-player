#ifndef __GAMSONG_MP3_H__
#define __GAMSONG_MP3_H__
#include <stdint.h>

// 7E FF 06 0F 00 01 01 xx xx EF
// 0	->	7E is start code
// 1	->	FF is version
// 2	->	06 is length
// 3	->	0F is command
// 4	->	00 is no receive
// 5~6	->	01 01 is argument
// 7~8	->	checksum = 0 - ( FF+06+0F+00+01+01 )
// 9	->	EF is end code

void mp3_set_reply (uint16_t state);


void fill_uint16_bigend (char *thebuf, uint16_t data);

//error because it is biggend mode in mp3 module
//void fill_uint16 (char *thebuf, uint16_t data) {
//        *(uint16_t*)(thebuf) = data;
//}

//void s_send_func (void);

//void mp3_send_cmd (void);
void mp3_send_cmd_num (char cmd, uint16_t arg);
void mp3_send_cmd (char cmd);

uint16_t mp3_get_checksum (char *thebuf);
void mp3_fill_checksum (void);

void mp3_send_cmd_num (char cmd, uint16_t arg);
void mp3_play_physical_num (uint16_t num);
void mp3_play_physical (void);

void mp3_next (void);
void mp3_prev (void);

//0x06 set volume 0-30
void mp3_set_volume (uint16_t volume);

//0x07 set EQ0/1/2/3/4/5    Normal/Pop/Rock/Jazz/Classic/Bass
void mp3_set_EQ (uint16_t eq);
//0x09 set device 1/2/3/4/5 U/SD/AUX/SLEEP/FLASH
void mp3_set_device (uint16_t device);

void mp3_sleep (void);
void mp3_reset (void);
void mp3_pause (void);
void mp3_stop (void);

void mp3_play (void);
//specify a mp3 file in mp3 folder in your tf card, "mp3_play (1);" mean play "mp3/0001.mp3"
void mp3_play_num (uint16_t num);

void mp3_get_state (void);
void mp3_get_volume (void);
void mp3_get_u_sum (void);
void mp3_get_tf_sum (void);
void mp3_get_flash_sum (void);
void mp3_get_tf_current (void);
void mp3_get_u_current (void);
void mp3_get_flash_current (void);

//set single loop
void mp3_single_loop (uint16_t state);
void mp3_single_play (uint16_t num);
void mp3_DAC (uint16_t state);
void mp3_random_play (void);

void mp3_select_folder(void);

#endif
