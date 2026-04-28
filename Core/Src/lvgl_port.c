#include "lvgl_port.h"
#include "gc9a01.h"

// Draw buffer — LVGL renders into this, then we flush it to the display
// Size: 1/10 of screen is a good balance between RAM and performance
#define BUF_SIZE (GC9A01_WIDTH * GC9A01_HEIGHT / 4)   
static lv_color_t lvgl_buf1[BUF_SIZE]; 

static volatile uint8_t flush_done = 1;

// ─── Flush callback ──────────────────────────────────────────────────────────
// LVGL calls this when it has rendered a region and wants it sent to the display
static void lvgl_flush_cb(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *buf)
{
    uint16_t w = area->x2 - area->x1 + 1;
    uint16_t h = area->y2 - area->y1 + 1;

    GC9A01_Flush(area->x1, area->y1, area->x2, area->y2,
                 (uint8_t *)buf, w * h * 2);

    lv_disp_flush_ready(drv);
}

// ─── Init ────────────────────────────────────────────────────────────────────
void LVGL_Port_Init(void)
{
    lv_init();

    // Register display buffer
    static lv_disp_draw_buf_t draw_buf;
    lv_disp_draw_buf_init(&draw_buf, lvgl_buf1, NULL, BUF_SIZE);

    // Register display driver
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res  = GC9A01_WIDTH;
    disp_drv.ver_res  = GC9A01_HEIGHT;
    disp_drv.flush_cb = lvgl_flush_cb;
    disp_drv.draw_buf = &draw_buf;
    disp_drv.full_refresh = 1;
    lv_disp_drv_register(&disp_drv);
}

// ─── Tick ────────────────────────────────────────────────────────────────────
// LVGL needs to know how much time has passed — call this every 1ms
void LVGL_Port_Tick(void)
{
    lv_tick_inc(1);
}