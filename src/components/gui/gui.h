/**
* @file gui.h

* @brief See the source file.
* 
* COPYRIGHT NOTICE: (c) 2022 Byte Lab Grupa d.o.o.
* All rights reserved.
*/

#ifndef __GUI_H__
#define __GUI_H__
#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------- INCLUDES ----------------------------------
#include "../lvgl/src/hal/lv_hal_disp.h"
#include "../lvgl/src/misc/lv_area.h"
#include "../lvgl/src/misc/lv_color.h"
//---------------------------------- MACROS -----------------------------------

//-------------------------------- DATA TYPES ---------------------------------

//---------------------- PUBLIC FUNCTION PROTOTYPES --------------------------

/**
 * @brief Initializes LVGL, TFT drivers and input drivers and starts task needed for GUI operation.
 * 
 */
void gui_init(void);

void custom_disp_driver_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p);
void toggle_invert_colors();

int is_inverted(void);


#ifdef __cplusplus
}
#endif

#endif // __GUI_H__
