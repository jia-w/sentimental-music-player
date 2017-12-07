#include <string.h>
#include <stdint.h>
#include "Gamsong_algorithm.h"
#include "Gamsong_usart.h"


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
