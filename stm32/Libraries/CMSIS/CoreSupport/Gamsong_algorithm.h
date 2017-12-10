#ifndef __GAMSONG_ALGORITHM_H__
#define __GAMSONG_ALGORITHM_H__

#include <stdint.h>

extern char GAMSONG_STRING[20];

int parseInt(char* str, int len);
void Delay(uint32_t us);
int compareStr(char* lhs, char* rhs);
void replaceStr(char* str, const char* replace);
int findStr(const char* find, char* search);
void jsonCopy(char* str, int SZ);

double get_discomfortIndex(void);
char* isWeather(char *s); // rainy, snowy, clear
int parse_year(char* time);
int parse_month(char* time);
int parse_day(char* time);
int parse_hour(char* time);
int parse_min(char* time);
int parse_sec(char* time);



void set_GamsongString(void);
int get_GamsongNum(void);

#endif
