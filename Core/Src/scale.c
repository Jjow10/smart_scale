#include "scale.h"

static int32_t tare_offset = 0;
static float scale_factor = 393.9f; //Hard coded

void scale_Tare(void){
    int64_t sum = 0;
    for(int i = 0; i < 8; i++)
        sum += hx711_Read();
    tare_offset = (int32_t)(sum / 8);
}

void scale_UpdateWeight(void){
    int64_t sum = 0;
    for(int i = 0; i < 3; i++)
        sum += hx711_Read();
    // Cast to float before dividing to avoid integer truncation
    stateSetWeight(((float)sum / 3.0f - (float)tare_offset) / scale_factor);
}