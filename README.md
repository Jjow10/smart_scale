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

## 1.28 inch LCD w/ GC9A01 Driver
### Pin Mapping

| Signal | Pin |
|---|---|
| SPI1 SCK | PA5 |
| SPI1 MOSI | PA7 |
| Display CS | PA6 |
| Display DC | PA9 |
| Display RST | PC7 |

LVGL v8.3 is used for this project and included as a submodule
Clone this Repo with 
```bash
git clone --recurse-submodules <your-repo-url>
```

If already cloned without submodules:

```bash
git submodule update --init --recursive
```
