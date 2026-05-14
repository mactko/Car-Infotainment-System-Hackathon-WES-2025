#include "wifi_post.h"

#include <string.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_http_client.h"
#include "nvs_flash.h"
#include "esp_netif.h"

#define WIFI_SSID           "Alberto"
#define WIFI_PASS           "albertoo"
#define POST_INTERVAL_MS    (60000U)
#define SERVER_URL          "http://albertokerim.com/data"

#define MAX_PAYLOAD_SIZE 256
static char payload_buffer[MAX_PAYLOAD_SIZE];

static const char *TAG = "WIFI_POST";
static const char *payload = payload_buffer;
static TaskHandle_t post_task_handle = NULL;

//------------------------- PRIVATE FUNCTION PROTOTYPES ------------------------
static esp_err_t _nvs_init(void);
static esp_err_t _wifi_init_sta(void);
static void _event_handler(void *p_arg, esp_event_base_t event_base, int32_t event_id, void *p_event_data);
static void _send_data_task(void *p_param);

//---------------------------- PUBLIC FUNCTIONS -------------------------------

esp_err_t wifi_post_start(void)
{
    ESP_LOGI(TAG, "Initializing Wi-Fi + POST...");

    ESP_ERROR_CHECK(_nvs_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &_event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &_event_handler, NULL));

    ESP_ERROR_CHECK(_wifi_init_sta());

    if (post_task_handle == NULL)
    {
        xTaskCreate(&_send_data_task, "send_data_task", 8192, NULL, 5, &post_task_handle);
    }

    return ESP_OK;
}

void wifi_post_set_payload(const char *json_data)
{
    if (json_data != NULL)
    {
        payload = json_data; // NOTE: Assumes pointer stays valid!
    }
}

void wifi_post_set_sensor_data(float temperature, int humidity)
{
    snprintf(payload_buffer, MAX_PAYLOAD_SIZE,
             "{\"temperature\":%.2f,\"humidity\":%d}",
             temperature, humidity);
}

//---------------------------- PRIVATE FUNCTIONS ------------------------------

static esp_err_t _nvs_init(void)
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    return ret;
}

static esp_err_t _wifi_init_sta(void)
{
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
        },
    };

    ESP_LOGI(TAG, "Setting Wi-Fi configuration...");
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    return ESP_OK;
}

static void _event_handler(void *p_arg, esp_event_base_t event_base, int32_t event_id, void *p_event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        ESP_LOGI(TAG, "Wi-Fi started, connecting...");
        esp_wifi_connect();
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        ESP_LOGW(TAG, "Wi-Fi disconnected, retrying...");
        esp_wifi_connect();
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        ESP_LOGI(TAG, "Got IP address!");
    }
}

static void _send_data_task(void *p_param)
{
    while (true)
    {
        esp_http_client_config_t config = {
            .url = SERVER_URL,
            .method = HTTP_METHOD_POST,
        };

        esp_http_client_handle_t client = esp_http_client_init(&config);
        esp_http_client_set_header(client, "Content-Type", "application/json");
        esp_http_client_set_post_field(client, payload, strlen(payload));

        esp_err_t err = esp_http_client_perform(client);
        if (err == ESP_OK)
        {
            int status = esp_http_client_get_status_code(client);
            ESP_LOGI(TAG, "POST sent! Status = %d", status);
        }
        else
        {
            ESP_LOGE(TAG, "POST failed: %s", esp_err_to_name(err));
        }

        esp_http_client_cleanup(client);
        vTaskDelay(POST_INTERVAL_MS / portTICK_PERIOD_MS);
    }
}
