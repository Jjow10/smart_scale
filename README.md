# Smart Scale

STM32-based pour-over coffee scale using an HX711 load cell amplifier.

## Hardware
- STM32F446RE (NUCLEO-F446RE)
- 5kg load cell + HX711 ADC module
- UART output via USART2 (115200 baud)

## Wiring
| Load Cell | HX711 |
|-----------|-------|
| Red       | E+    |
| Black     | E-    |
| White     | A-    |
| Green     | A+    |

| HX711 | STM32 |
|-------|-------|
| DOUT  | PA0   |
| SCK   | PA1   |
| VCC   | 3.3V  |
| GND   | GND   |

## Calibration
Scale factor is hardcoded in `Core/Src/hx711.c` (`scale_factor = 393.9f`).
If load cell is replaced recalibrate.
