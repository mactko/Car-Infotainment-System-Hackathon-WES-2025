#ifndef WIFI_POST_H
#define WIFI_POST_H

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initializes Wi-Fi and starts periodic HTTP POST task.
 * 
 * Call this once after app startup (e.g., after LVGL init).
 */
esp_err_t wifi_post_start(void);

/**
 * @brief Update sensor data (optional).
 * 
 * If your sensor logic runs elsewhere, use this to inject JSON payload.
 * @param json_data Pointer to a null-terminated JSON string (must remain valid until next update).
 */
void wifi_post_set_payload(const char *json_data);

/**
 * @brief Sets sensor readings and builds JSON payload.
 *
 * @param temperature Float temperature value.
 * @param humidity Integer humidity value.
 */
void wifi_post_set_sensor_data(float temperature, int humidity);

#ifdef __cplusplus
}
#endif

#endif // WIFI_POST_H