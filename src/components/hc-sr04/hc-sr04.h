#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"             
#include "esp_rom_sys.h"           


#define TRIG_PIN 26
#define ECHO_PIN 27
#define TAG "HC-SR04"

void hcsr04_init(void);
void hcsr04_read_distance(float *distance);
