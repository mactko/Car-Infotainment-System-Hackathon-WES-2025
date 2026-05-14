/**
* @file main.c

* @brief
*
* COPYRIGHT NOTICE: (c) 2025 Byte Lab Grupa d.o.o.
* All rights reserved.
*/

//--------------------------------- INCLUDES ----------------------------------

//---------------------------------- MACROS -----------------------------------

//-------------------------------- DATA TYPES ---------------------------------

//---------------------- PRIVATE FUNCTION PROTOTYPES --------------------------

//------------------------- STATIC DATA & CONSTANTS ---------------------------

//------------------------------- GLOBAL DATA ---------------------------------

//------------------------------ PUBLIC FUNCTIONS -----------------------------
#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"


#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "driver/i2c.h"

#include "sensors.h"

void i2c_master_setup(void)
{
    i2c_config_t conf;

    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = SDA_PIN;
    conf.scl_io_num = SCL_PIN;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
    conf.clk_flags = 0;

    i2c_param_config(I2C_MASTER_NUM, &conf);
    i2c_driver_install(I2C_MASTER_NUM, I2C_MODE_MASTER, 0, 0, 0);
}

void app_main(void)
{
    gui_init();
    
    sensors_init();

    while(true) {

        vTaskDelay(1000 / portTICK_PERIOD_MS);

    }

    //i2c_master_setup();
    

    /*

    float distance = 0.f;
    float humidity = 0.f;float temperature = 0.f;

    hcsr04_init();

    hcsr04_read_distance(&distance);

    sht31_init();

    tcrt5000_init();

    veml7700_handle_t veml7700_dev;

    ESP_ERROR_CHECK(veml7700_initialize(&veml7700_dev, I2C_MASTER_NUM));

    while (true) {
        double lux_als, lux_white, fc_als, fc_white;

        // Read the ALS data
        ESP_ERROR_CHECK(veml7700_read_als_lux_auto(veml7700_dev, &lux_als) );
        // Convert to foot candles
        fc_als = lux_als * LUX_FC_COEFFICIENT;

        // Read the White data
        ESP_ERROR_CHECK( veml7700_read_white_lux_auto(veml7700_dev, &lux_white) );
        // Convert to foot candles
        fc_white = lux_white * LUX_FC_COEFFICIENT;

        printf("VEML7700 measured ALS %0.4f lux or %0.4f fc \n", lux_als, fc_als);
        printf("VEML7700 measured White %0.4f lux or %0.4f fc \n\n", lux_white, fc_white);

        hcsr04_read_distance(&distance);

        printf("Distance from sensor %0.4f \n\n", distance);

        while(sht31_read_temp_humi(&temperature, &humidity) != ESP_OK);

        printf("Temperature: %0.4f Humidity: %0.4f\n\n", temperature, humidity);

        int val = gpio_get_level(IR_PIN);
        if (val == 0) {
            printf("Object detected!\n");
        } else {
            printf("No object.\n");
        }

        sprintf(array, "Temperature:    %0.2f°C", temperature);

        lv_label_set_text(ui_TempsText, array);

        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
        */
}


//---------------------------- PRIVATE FUNCTIONS ------------------------------

//---------------------------- INTERRUPT HANDLERS -----------------------------
