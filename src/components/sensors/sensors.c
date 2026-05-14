#include "sensors.h"

static TimerHandle_t SENSOR_timer = NULL;
//static TimerHandle_t US_timer = NULL;

veml7700_handle_t veml7700_dev;

float humidity = 0.f;
float temperature = 0.f;

double lux_als = 0.f;
double fc_als = 0.f;

int state = 0;
int radio_state = 0;

static char array[32];
static char distance_array[32];

float distance = 0.f;

extern lv_obj_t * ui_red;
extern lv_obj_t * ui_yellow;
extern lv_obj_t * ui_green;

static void sensor_cb(TimerHandle_t handle);
//static void us_cb();

void bar_update_task(void *arg)
{
    while(1)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void dark_mode(void *arg)
{
    for(;;)
    {
        ESP_ERROR_CHECK(veml7700_read_als_lux_auto(veml7700_dev, &lux_als) );

        fc_als = lux_als * LUX_FC_COEFFICIENT;

        if((fc_als < ALS_THRESHOLD && !is_inverted()) || (fc_als > ALS_THRESHOLD && is_inverted())) {
            toggle_invert_colors();
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS); 
    }
}

void us_task(void *arg) {
    for(;;)
    {
        hcsr04_read_distance(&distance);

        sprintf(distance_array, "Distance: %0.fcm", distance);

        lv_label_set_text(ui_Label1, distance_array);

        lv_obj_add_flag(ui_green, LV_OBJ_FLAG_HIDDEN);
        if(state) {
            if(distance <= DISTANCE_SHORT) {
                lv_obj_add_flag(ui_green, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(ui_yellow, LV_OBJ_FLAG_HIDDEN);
                lv_obj_clear_flag(ui_red, LV_OBJ_FLAG_HIDDEN);
                buzzer_send(BUZZER_LONG);
            } else if(distance <= DISTANCE_MID) {
                lv_obj_add_flag(ui_green, LV_OBJ_FLAG_HIDDEN);
                lv_obj_clear_flag(ui_yellow, LV_OBJ_FLAG_HIDDEN);
                lv_obj_clear_flag(ui_red, LV_OBJ_FLAG_HIDDEN);
                buzzer_send(BUZZER_MID);
            } else {
                lv_obj_clear_flag(ui_green, LV_OBJ_FLAG_HIDDEN);
                lv_obj_clear_flag(ui_yellow, LV_OBJ_FLAG_HIDDEN);
                lv_obj_clear_flag(ui_red, LV_OBJ_FLAG_HIDDEN);
                buzzer_send(BUZZER_NONE);
            }
        }
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}

void sensors_init(void)
{
    ESP_ERROR_CHECK(sht31_init());

    hcsr04_init();
    buzzer_init();
    radio_init();

    ESP_ERROR_CHECK(veml7700_initialize(&veml7700_dev, I2C_MASTER_NUM));

    SENSOR_timer = xTimerCreate("Sensor_timer", (SENSOR_REPORTING_PERIOD * 1000) / portTICK_PERIOD_MS,
                            pdTRUE, NULL, sensor_cb);

    xTimerStart(SENSOR_timer, 0);

    //US_timer = xTimerCreate("US_timer", (US_PERIOD * 1000) / portTICK_PERIOD_MS, pdTRUE, NULL, us_cb);

    //xTimerStart(US_timer, 0);

    xTaskCreatePinnedToCore(dark_mode, "dark_mode", 2048, NULL, 1, NULL, 1);

    xTaskCreatePinnedToCore(us_task, "us_task", 2048, NULL, 1, NULL, 1);
}


static void sensor_cb(TimerHandle_t handle){
    
    if (sht31_read_temp_humi(&temperature, &humidity) == ESP_OK) {
        sprintf(array, "Temperature: %0.f°C", temperature);
    }

    lv_label_set_text(ui_TempsText, array);
}

/*
static void us_cb(){
    
    hcsr04_read_distance(&distance);

    lv_obj_add_flag(ui_green, LV_OBJ_FLAG_HIDDEN);
    
    if(state) {
        if(distance <= DISTANCE_SHORT) {
            lv_obj_add_flag(ui_green, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ui_yellow, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_red, LV_OBJ_FLAG_HIDDEN);
        } else if(distance <= DISTANCE_MID) {
            lv_obj_add_flag(ui_green, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_yellow, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_red, LV_OBJ_FLAG_HIDDEN);
        } else {
            lv_obj_clear_flag(ui_green, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_yellow, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_red, LV_OBJ_FLAG_HIDDEN);
        }
    }
}
*/


    