
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"             
#include "esp_rom_sys.h"           
#include "hc-sr04.h"

void hcsr04_init() {

    // Configure TRIG pin as output
    gpio_reset_pin(TRIG_PIN);
    gpio_set_direction(TRIG_PIN, GPIO_MODE_OUTPUT);

    // Configure ECHO pin as input
    gpio_reset_pin(ECHO_PIN);
    gpio_set_direction(ECHO_PIN, GPIO_MODE_INPUT);
}

void hcsr04_read_distance(float *distance) {
    gpio_set_level(TRIG_PIN, 0);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    gpio_set_level(TRIG_PIN, 1);
    esp_rom_delay_us(10);  // Fixed delay
    gpio_set_level(TRIG_PIN, 0);

    // Wait for ECHO to go HIGH
    while (gpio_get_level(ECHO_PIN) == 0) {}
    int64_t start_time = esp_timer_get_time();  // Fixed timer

    // Wait for ECHO to go LOW
    while (gpio_get_level(ECHO_PIN) == 1) {}
    int64_t end_time = esp_timer_get_time();

    int64_t duration = end_time - start_time;
    *distance = (float)duration / 58.0;
}


