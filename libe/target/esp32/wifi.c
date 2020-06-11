/*
 * WiFi support for esp32.
 */

#ifdef USE_WIFI

#include <libe/libe.h>
#include <string.h>
#include <stdlib.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/event_groups.h>
#include <esp_wifi.h>
#include <esp_wpa2.h>
#include <esp_event.h>
#include <esp_log.h>
#include <esp_system.h>
#include <nvs_flash.h>
#include <esp_netif.h>
#include <esp_smartconfig.h>


/* FreeRTOS event group to signal when we are connected & ready to make a request */
static EventGroupHandle_t wifi_event_group;
static const int WIFI_CONNECTED_BIT = BIT0;
static const int WIFI_SMARTCONFIG_BIT = BIT1;
static const int WAIT_CONNECTED_BIT = BIT2;
static const int WAIT_SMARTCONFIG_DONE_BIT = BIT3;


static void wifi_smartconfig_task(void * parm)
{
	EventBits_t uxBits;
	ESP_ERROR_CHECK(esp_smartconfig_set_type(SC_TYPE_ESPTOUCH));
	smartconfig_start_config_t cfg = SMARTCONFIG_START_CONFIG_DEFAULT();
	ESP_ERROR_CHECK(esp_smartconfig_start(&cfg));
	while (1) {
		uxBits = xEventGroupWaitBits(wifi_event_group, WAIT_CONNECTED_BIT | WAIT_SMARTCONFIG_DONE_BIT, true, false, portMAX_DELAY);
		if (uxBits & WAIT_CONNECTED_BIT) {
			DEBUG_MSG("connected to access point");
		}
		if (uxBits & WAIT_SMARTCONFIG_DONE_BIT) {
			DEBUG_MSG("smartconfig over");
			esp_smartconfig_stop();
			xEventGroupClearBits(wifi_event_group, WIFI_SMARTCONFIG_BIT);
			vTaskDelete(NULL);
		}
	}
}

static void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
	if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
		DEBUG_MSG("start automatic connect");
		esp_wifi_connect();
	} else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
		if (!(xEventGroupGetBits(wifi_event_group) & WIFI_SMARTCONFIG_BIT)) {
			esp_wifi_connect();
		}
		xEventGroupClearBits(wifi_event_group, WIFI_CONNECTED_BIT | WAIT_CONNECTED_BIT);
		DEBUG_MSG("wifi disconnected");
	} else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
		xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT | WAIT_CONNECTED_BIT);
		DEBUG_MSG("wifi got ip");
	} else if (event_base == SC_EVENT && event_id == SC_EVENT_SCAN_DONE) {
		DEBUG_MSG("scan done");
	} else if (event_base == SC_EVENT && event_id == SC_EVENT_FOUND_CHANNEL) {
		DEBUG_MSG("found channel");
	} else if (event_base == SC_EVENT && event_id == SC_EVENT_GOT_SSID_PSWD) {
		smartconfig_event_got_ssid_pswd_t *evt = (smartconfig_event_got_ssid_pswd_t *)event_data;

		wifi_config_t wifi_config;
		memset(&wifi_config, 0, sizeof(wifi_config_t));

		memcpy(wifi_config.sta.ssid, evt->ssid, sizeof(wifi_config.sta.ssid));
		memcpy(wifi_config.sta.password, evt->password, sizeof(wifi_config.sta.password));

		wifi_config.sta.bssid_set = evt->bssid_set;
		if (wifi_config.sta.bssid_set == true) {
			memcpy(wifi_config.sta.bssid, evt->bssid, sizeof(wifi_config.sta.bssid));
		}

		DEBUG_MSG("ssid: %s, password: %s", evt->ssid, evt->password);

		ESP_ERROR_CHECK(esp_wifi_disconnect());
		ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
		ESP_ERROR_CHECK(esp_wifi_connect());
	} else if (event_base == SC_EVENT && event_id == SC_EVENT_SEND_ACK_DONE) {
		xEventGroupSetBits(wifi_event_group, WAIT_SMARTCONFIG_DONE_BIT);
	}
}

int wifi_init(void)
{
	ERROR_IF_R(esp_netif_init() != ESP_OK, -1, "esp_netif_init() failed");
	wifi_event_group = xEventGroupCreate();
	ERROR_IF_R(esp_event_loop_create_default(), -1, "esp_event_loop_create_default() failed");
	esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
	ERROR_IF_R(!sta_netif, -1, "esp_netif_create_default_wifi_sta() failed");

	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
	ERROR_IF_R(esp_wifi_init(&cfg) != ESP_OK, -1, "esp_wifi_init() failed");

	esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL);
	esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL);
	esp_event_handler_register(SC_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL);

	ERROR_IF_R(esp_wifi_set_mode(WIFI_MODE_STA), -1, "esp_wifi_set_mode() failed");

	return 0;
}

void wifi_quit(void)
{
	esp_wifi_disconnect();
	esp_wifi_stop();
	esp_wifi_deinit();
}

int wifi_connect(char *ssid, char *password)
{
	if (wifi_connected()) {
		WARN_MSG("wifi already connected");
		return -1;
	}

	ERROR_IF_R(esp_wifi_start(), -1, "esp_wifi_start() failed");
	if (ssid && password) {
		wifi_config_t wifi_config;
		memset(&wifi_config, 0, sizeof(wifi_config_t));

		strncpy((char *)wifi_config.sta.ssid, ssid, sizeof(wifi_config.sta.ssid) - 1);
		strncpy((char *)wifi_config.sta.password, password, sizeof(wifi_config.sta.password) - 1);

		DEBUG_MSG("ssid: %s, password: %s", ssid, password);

		ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
		ESP_ERROR_CHECK(esp_wifi_connect());
	}

	return 0;
}

void wifi_disconnect(void)
{
	esp_wifi_disconnect();
	esp_wifi_stop();
}

bool wifi_connected(void)
{
	return xEventGroupGetBits(wifi_event_group) & WIFI_CONNECTED_BIT ? true : false;
}

int wifi_smartconfig(bool force)
{
	if (xEventGroupGetBits(wifi_event_group) & WIFI_SMARTCONFIG_BIT) {
		WARN_MSG("smartconfig already in progress");
		return -1;
	}

	if (!wifi_connected() || force) {
		xEventGroupSetBits(wifi_event_group, WIFI_SMARTCONFIG_BIT);
		esp_wifi_disconnect();
		xTaskCreate(wifi_smartconfig_task, "wifi_smartconfig_task", 4096, NULL, 3, NULL);
		return 0;
	}

	WARN_MSG("wifi already connected, will not start smartconfig without force = true");
	return -1;
}

bool wifi_smartconfig_in_progress(void)
{
	return xEventGroupGetBits(wifi_event_group) & WIFI_SMARTCONFIG_BIT ? true : false;
}

#endif
