#include "buzzer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/ledc.h"

#define BUZZER_PIN     2
#define LEDC_TIMER     LEDC_TIMER_0
#define LEDC_MODE      LEDC_LOW_SPEED_MODE
#define LEDC_CHANNEL   LEDC_CHANNEL_0
#define DUTY_RES       LEDC_TIMER_10_BIT
#define DUTY_CYCLE     512

static QueueHandle_t buzzer_event_queue;

static void init_buzzer_pwm(void) {
    ledc_timer_config_t timer = {
        .speed_mode       = LEDC_MODE,
        .duty_resolution  = DUTY_RES,
        .timer_num        = LEDC_TIMER,
        .freq_hz          = 2000,
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ledc_timer_config(&timer);

    ledc_channel_config_t channel = {
        .gpio_num       = BUZZER_PIN,
        .speed_mode     = LEDC_MODE,
        .channel        = LEDC_CHANNEL,
        .timer_sel      = LEDC_TIMER,
        .duty           = 0,
        .hpoint         = 0
    };
    ledc_channel_config(&channel);
}

static void play_buzzer(uint16_t freq, uint16_t duration_ms) {
    ledc_set_freq(LEDC_MODE, LEDC_TIMER, freq);
    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, DUTY_CYCLE);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
    vTaskDelay(pdMS_TO_TICKS(duration_ms));
    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, 0);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
}

static void buzzer_task(void *arg) {
    buzzer_event_type_t evt;
    while (1) {
        if (xQueueReceive(buzzer_event_queue, &evt, portMAX_DELAY)) {
            switch (evt) {
                case BUZZER_LONG:
                    for (int i = 0; i < 3; i++) {
                        play_buzzer(3000, 1000); // Long beep
                        vTaskDelay(pdMS_TO_TICKS(200));
                    }
                    break;
                case BUZZER_MID:
                    for (int i = 0; i < 3; i++) {
                        play_buzzer(2000, 300);  // Mid beep
                        vTaskDelay(pdMS_TO_TICKS(150));
                    }
                    break;
                case BUZZER_NONE:
                default:
                    // No beep
                    break;
            }
        }
    }
}

void buzzer_init(void) {
    init_buzzer_pwm();
    buzzer_event_queue = xQueueCreate(4, sizeof(buzzer_event_type_t));
    xTaskCreate(buzzer_task, "buzzer_task", 2048, NULL, 5, NULL);
}

void buzzer_send(buzzer_event_type_t type) {
    xQueueSend(buzzer_event_queue, &type, 0);
}
