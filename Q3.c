#include <stdio.h>
#include <stdlib.h>

typedef struct{
    unsigned char max;
    unsigned char min;
    unsigned char avg;
    unsigned char* even_array;
    int size;
}array_result_t;

array_result_t result;
void calc_array(unsigned char*, unsigned char, array_result_t*);

void main (void){
    int i = 0;
    //test array
    unsigned char ar[10] = {10,21,23,43,17,53,32,33,13,2};
    calc_array(&ar[0], 10, &result);
    
    //display array
    for(i=0; i< result.size; i++)
        printf("%d", result.even_array++);
}

void calc_array(unsigned char* buff, unsigned char size, array_result_t* res)
{
    unsigned char min, max, cnt, even_cnt;
    int avg;

    avg = max = cnt = even_cnt = 0;
    min = 0xff;

    while(cnt++ < size){
        if(*buff < min)
            min = *buff;
        if(*buff > max)
            max = *buff;
        if(*buff % 2)
            even_cnt++;
        avg+=*buff;
        buff++;
    }
    buff-=size;
    res->even_array = (unsigned char*) malloc(even_cnt*sizeof(unsigned char));

    cnt = 0;
    while(cnt++ < size){
        if(*buff % 2)
        {
            *res->even_array = *buff;
            res->even_array++;
        }
        buff++;          
    }
    res->max = max;
    res->min = min;
    res->avg = avg/size;
    res->size = even_cnt;
    res->even_array -= res->size;
}