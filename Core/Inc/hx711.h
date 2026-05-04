/*
 * hx711.h
 *
 *  Created on: Apr 21, 2026
 *      Author: joeyw
 */

#ifndef INC_HX711_H_
#define INC_HX711_H_

#include "main.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define HX711_DOUT_PORT GPIOA
#define HX711_DOUT_PIN GPIO_PIN_0

#define HX711_SCK_PORT GPIOA
#define HX711_SCK_PIN GPIO_PIN_1

#define FLASH_SCALE_ADDR 0x08060000UL
#define FLASH_SECTOR_NUMBER FLASH_SECTOR_7

#define FLASH_MAGIC 0xBEEF1234UL

int32_t hx711_Read(void);

void hx711_Init(void);

#endif /* INC_HX711_H_ */
