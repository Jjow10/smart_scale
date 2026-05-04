#include "scale.h"

static int32_t tare_offset = 0;
static float scale_factor = 393.9f; //Hard coded

void scale_Tare(void){
    int64_t sum = 0;
    for(int i = 0; i < 16; i++)
        sum += hx711_Read();
    tare_offset = (int32_t)(sum / 16);
}

void scale_UpdateWeight(void){
    int32_t raw = hx711_Read();
    stateSetWeight(((float)raw - (float)tare_offset) / scale_factor);
}