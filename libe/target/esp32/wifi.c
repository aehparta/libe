/*
 * WiFi support for esp32.
 */

#ifdef USE_WIFI

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/event_groups.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <esp_wps.h>
#include <esp_event_loop.h>
#include <libe/log.h>
#include <libe/wifi.h>
#include "wifi.h"


static EventGroupHandle_t wifi_event_group;
static esp_wps_config_t wifi_wps_config = WPS_CONFIG_INIT_DEFAULT(WPS_TYPE_PBC);


static esp_err_t wifi_event_handler(void *ctx, system_event_t *event)
{
	static int wifi_retries = 0;

	switch (event->event_id) {
	case SYSTEM_EVENT_STA_START:
		if (esp_wifi_connect()) {
			ERROR_IF_R(esp_wifi_wps_enable(&wifi_wps_config), ESP_FAIL, "wifi wps enable failed");
			ERROR_IF_R(esp_wifi_wps_start(0), ESP_FAIL, "wifi wps start failed");
			DEBUG_MSG("wifi wps setup started");
		} else {
			DEBUG_MSG("wifi is connecting");
		}
		break;
	case SYSTEM_EVENT_STA_GOT_IP:
		INFO_MSG("wifi got ip: %s", ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip));
		ERROR_IF_R(esp_wifi_wps_disable(), ESP_FAIL, "wifi wps disable failed");
		wifi_retries = 0;
		xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
		break;
	case SYSTEM_EVENT_STA_DISCONNECTED:
		if (wifi_retries < WIFI_MAXIMUM_RETRY) {
			esp_wifi_connect();
			xEventGroupClearBits(wifi_event_group, WIFI_CONNECTED_BIT);
			wifi_retries++;
			WARN_MSG("wifi retry connect to the AP");
		} else {
			ERROR_IF_R(esp_wifi_wps_disable(), ESP_FAIL, "wifi wps disable after disconnect failed");
			ERROR_IF_R(esp_wifi_wps_enable(&wifi_wps_config), ESP_FAIL, "wifi wps enable after disconnect failed");
			ERROR_IF_R(esp_wifi_wps_start(0), ESP_FAIL, "wifi wps start after disconnect failed");
			wifi_retries = 0;
		}
		WARN_MSG("wifi connect to the AP fail");
		break;
	case SYSTEM_EVENT_STA_WPS_ER_SUCCESS:
		/*point: the function esp_wifi_wps_start() only get ssid & password
		 * so call the function esp_wifi_connect() here
		 * */
		DEBUG_MSG("SYSTEM_EVENT_STA_WPS_ER_SUCCESS");
		ERROR_IF_R(esp_wifi_wps_disable(), ESP_FAIL, "wifi wps disable failed");
		ERROR_IF_R(esp_wifi_connect(), ESP_FAIL, "wifi connect after wps setup failed");
		break;
	case SYSTEM_EVENT_STA_WPS_ER_FAILED:
		DEBUG_MSG("SYSTEM_EVENT_STA_WPS_ER_FAILED");
		ERROR_IF_R(esp_wifi_wps_disable(), ESP_FAIL, "wifi wps disable after failed wps attempt failed");
		ERROR_IF_R(esp_wifi_wps_enable(&wifi_wps_config), ESP_FAIL, "wifi wps enable after failed wps attempt failed");
		ERROR_IF_R(esp_wifi_wps_start(0), ESP_FAIL, "wifi wps start after failed wps attempt failed");
		break;
	case SYSTEM_EVENT_STA_WPS_ER_TIMEOUT:
		DEBUG_MSG("SYSTEM_EVENT_STA_WPS_ER_TIMEOUT");
		ERROR_IF_R(esp_wifi_wps_disable(), ESP_FAIL, "wifi wps disable after wps timeout failed");
		ERROR_IF_R(esp_wifi_wps_enable(&wifi_wps_config), ESP_FAIL, "wifi wps enable after wps timeout failed");
		ERROR_IF_R(esp_wifi_wps_start(0), ESP_FAIL, "wifi wps start after wps timeout failed");
		break;
	default:
		break;
	}

	return ESP_OK;
}

int wifi_init(void)
{
	wifi_event_group = xEventGroupCreate();

	tcpip_adapter_init();
	ERROR_IF_R(esp_event_loop_init(wifi_event_handler, NULL), -1, "wifi event loop setup failed");

	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
	ERROR_IF_R(esp_wifi_init(&cfg), -1, "wifi config has errors");

	ERROR_IF_R(esp_wifi_set_mode(WIFI_MODE_STA), -1, "unable to set wifi mode to WIFI_MODE_STA");
	ERROR_IF_R(esp_wifi_start(), -1, "unable to start wifi");

	return 0;
}

void wifi_quit(void)
{

}

bool wifi_connected(void)
{
	return xEventGroupGetBits(wifi_event_group) & WIFI_CONNECTED_BIT ? true : false;
}

#endif
