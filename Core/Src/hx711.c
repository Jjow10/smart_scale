/*
 * hx711.c
 */

#include "hx711.h"

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
    for(int i = 0; i < 10; i++){
        hx711_Read();
        HAL_Delay(10); // HX711 outputs ~10 SPS at default rate
    }
}

