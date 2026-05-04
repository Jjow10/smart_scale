#include "ui.h"

static lv_obj_t *spinner_arc;
static lv_obj_t *main_label;
static lv_obj_t *sub_label;

static void anim_arc_rotate(void *obj, int32_t val){
    lv_arc_set_rotation((lv_obj_t *)obj, (uint16_t)(val % 360));
}

static void update_weight_cb(lv_timer_t *timer){
    lv_label_set_text_fmt(sub_label, "%.1f g", stateGetWeight());
}

void SmartScale_BootAnim(void)
{
    lv_obj_t *scr = lv_scr_act();
    lv_obj_set_style_bg_color(scr, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, 0);
    lv_obj_clear_flag(scr, LV_OBJ_FLAG_SCROLLABLE);

    /* Single clean spinner arc */
    spinner_arc = lv_arc_create(scr);
    lv_obj_set_size(spinner_arc, 200, 200);
    lv_obj_align(spinner_arc, LV_ALIGN_CENTER, 0, 0);
    lv_arc_set_angles(spinner_arc, 0, 90);         // 90° visible segment
    lv_arc_set_bg_angles(spinner_arc, 0, 360);
    lv_obj_set_style_arc_color(spinner_arc, lv_color_hex(0x00E5FF), LV_PART_INDICATOR);
    lv_obj_set_style_arc_width(spinner_arc, 6, LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(spinner_arc, lv_color_hex(0x1A1A1A), LV_PART_MAIN);
    lv_obj_set_style_arc_width(spinner_arc, 6, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(spinner_arc, LV_OPA_TRANSP, 0);
    lv_obj_remove_style(spinner_arc, NULL, LV_PART_KNOB);

    /* Title */
    main_label = lv_label_create(scr);
    lv_label_set_text(main_label, "SMART SCALE");
    lv_obj_set_style_text_color(main_label, lv_color_white(), 0);
    lv_obj_set_style_text_letter_space(main_label, 3, 0);
    lv_obj_align(main_label, LV_ALIGN_CENTER, 0, -10);

    sub_label = lv_label_create(scr);
    float weight = stateGetWeight();
    lv_label_set_text_fmt(sub_label, "%.1f g", weight);
    lv_obj_set_style_text_color(sub_label, lv_color_white(), 0);
    lv_obj_align(sub_label, LV_ALIGN_CENTER, 0, 15);

    lv_timer_create(update_weight_cb, 100, NULL);

    /* Spin the arc */
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_exec_cb(&a, anim_arc_rotate);
    lv_anim_set_var(&a, spinner_arc);
    lv_anim_set_values(&a, 0, 360);
    lv_anim_set_time(&a, 1000);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    lv_anim_set_path_cb(&a, lv_anim_path_linear);
    lv_anim_start(&a);
}