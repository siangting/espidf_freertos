#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "esp_wifi.h"

static const char *TAG = "INIT_PROF";

/* 
 * MEASURE 巨集：包裹任意 esp_err_t 函式呼叫
 * 前後用 esp_timer_get_time() 量測，並印出微秒差
 */
#define MEASURE(desc, call) do {                          \
    int64_t _t0 = esp_timer_get_time();                   \
    esp_err_t _err = call;                                \
    int64_t _t1 = esp_timer_get_time();                   \
    ESP_LOGI(TAG, "%s took %lld µs%s",                    \
             desc, (_t1 - _t0),                          \
             (_err == ESP_OK) ? "" : " (error)");        \
} while(0)

void app_main(void)
{
    /* 開啟這個 tag 的 INFO log */
    esp_log_level_set(TAG, ESP_LOG_INFO);

    /* 1. 初始化 NVS */
    MEASURE("nvs_flash_init", nvs_flash_init());

    /* 2. 初始化 TCP/IP stack 和 default event loop */
    MEASURE("esp_netif_init", esp_netif_init());
    MEASURE("esp_event_loop_create_default", esp_event_loop_create_default());

    /* 3. 初始化 WiFi */
    {
        MEASURE("wifi_init", ({
            wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
            esp_wifi_init(&cfg);
        }));
        MEASURE("wifi_set_mode", esp_wifi_set_mode(WIFI_MODE_STA));
        MEASURE("wifi_start", esp_wifi_start());
    }

    ESP_LOGI(TAG, "Dynamic initialization done.");

    /* 進入空迴圈 */
    while (true) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
