#include "radio.h"

void radio_init(void)
{
    gpio_config_t io_conf = {
        .pin_bit_mask = 1ULL << ESP32_B_EN_GPIO,
        .mode = GPIO_MODE_OUTPUT,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io_conf);
}
