/*
 * hx711.c
 */

#include "hx711.h"

static int32_t tare_offset = 0;
static float scale_factor = 393.9f; //Hard coded

static inline void SCK_HIGH(void){
    HAL_GPIO_WritePin(HX711_SCK_PORT, HX711_SCK_PIN, GPIO_PIN_SET);
}
static inline void SCK_LOW(void){
    HAL_GPIO_WritePin(HX711_SCK_PORT, HX711_SCK_PIN, GPIO_PIN_RESET);
}
static inline bool DOUT_READ(void){
    return HAL_GPIO_ReadPin(HX711_DOUT_PORT, HX711_DOUT_PIN) == GPIO_PIN_SET;
}


int32_t hx711_Read(void){
    uint32_t raw = 0;

    while(DOUT_READ()){}  // wait for conversion to be ready

    for(int i = 0; i < 24; i++){
		SCK_HIGH();
		__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();
		raw = (raw << 1) | (DOUT_READ() ? 1U : 0U);  // back to reading while SCK is HIGH
		SCK_LOW();
		__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();
	}

    // 25th pulse 
    SCK_HIGH();
    __NOP();__NOP();__NOP();__NOP();__NOP();
    SCK_LOW();

    // Sign-extend 24-bit two's complement to 32-bit
    if(raw & 0x800000)
        raw |= 0xFF000000;

    return (int32_t)raw;
}

void hx711_Init(void){
    hx711_Read();
    hx711_Tare();
}

void hx711_Tare(void){
    int64_t sum = 0;
    for(int i = 0; i < 8; i++)
        sum += hx711_Read();
    tare_offset = (int32_t)(sum / 8);
}

float hx711_GetWeight(void){
    int64_t sum = 0;
    for(int i = 0; i < 3; i++)
        sum += hx711_Read();
    // Cast to float before dividing to avoid integer truncation
    return ((float)sum / 3.0f - (float)tare_offset) / scale_factor;
}

int32_t hx711_GetTareOffset(void){
    return tare_offset;
}