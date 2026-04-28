#ifndef LVGL_PORT_H
#define LVGL_PORT_H

#include "lvgl.h"

void LVGL_Port_Init(void);
void LVGL_Port_Tick(void);   // call this from SysTick or a 1ms timer

#endif