#ifndef __GAMSONG_ALGORITHM_H__
#define __GAMSONG_ALGORITHM_H__

#include <stdint.h>

void Delay(uint32_t us);
int compareStr(char* lhs, char* rhs);
int findStr(const char* find, char* search);
void jsonCopy(char* str, int SZ);

#endif
