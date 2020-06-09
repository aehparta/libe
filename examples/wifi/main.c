/*
 * WiFi example.
 */

#include <libe/libe.h>
#include <esp_log.h>
#include <mdns.h>
#include <mqtt_client.h>


#define WIFI_CONFIG_BUTTON_GPIO 13


#ifdef DO_MQTT_TEST

#define MQTT_SERVICE_NAME       "_mqtt"
#define MQTT_PROTO_NAME         "_tcp"

#define MQTT_URI                "mqtt://dev.lan:1883"


static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
	esp_mqtt_event_handle_t event = event_data;
	esp_mqtt_client_handle_t client = event->client;
	int msg_id;

	switch (event->event_id) {
	case MQTT_EVENT_CONNECTED:
		INFO_MSG("MQTT_EVENT_CONNECTED");
		msg_id = esp_mqtt_client_publish(client, "topic/qos1", "data_3", 0, 1, 0);
		INFO_MSG("sent publish successful, msg_id=%d", msg_id);
		msg_id = esp_mqtt_client_subscribe(client, "topic/qos0", 0);
		INFO_MSG("sent subscribe successful, msg_id=%d", msg_id);
		msg_id = esp_mqtt_client_subscribe(client, "topic/qos1", 1);
		INFO_MSG("sent subscribe successful, msg_id=%d", msg_id);
		msg_id = esp_mqtt_client_unsubscribe(client, "topic/qos1");
		INFO_MSG("sent unsubscribe successful, msg_id=%d", msg_id);
		break;
	case MQTT_EVENT_DISCONNECTED:
		INFO_MSG("MQTT_EVENT_DISCONNECTED");
		break;

	case MQTT_EVENT_SUBSCRIBED:
		INFO_MSG("MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
		msg_id = esp_mqtt_client_publish(client, "topic/qos0", "data", 0, 0, 0);
		INFO_MSG("sent publish successful, msg_id=%d", msg_id);
		break;
	case MQTT_EVENT_UNSUBSCRIBED:
		INFO_MSG("MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
		break;
	case MQTT_EVENT_PUBLISHED:
		INFO_MSG("MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
		break;
	case MQTT_EVENT_DATA:
		INFO_MSG("MQTT_EVENT_DATA");
		printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
		printf("DATA=%.*s\r\n", event->data_len, event->data);
		break;
	case MQTT_EVENT_ERROR:
		INFO_MSG("MQTT_EVENT_ERROR");
		break;
	default:
		INFO_MSG("Other event id:%d", event->event_id);
		break;
	}
}

void mqtt_task(void *p)
{
	/* wait for wifi to connect */
	while (!wifi_connected()) {
		os_delay_ms(100);
	}

#ifndef MQTT_URI
	/* try to resolve mqtt server */
	INFO_MSG("wifi connected, query mDNS to find mqtt server (%s.%s)", MQTT_SERVICE_NAME, MQTT_PROTO_NAME);
	mdns_result_t *results = NULL;
	mdns_init();
	esp_err_t err = mdns_query_ptr(MQTT_SERVICE_NAME, MQTT_PROTO_NAME, 3000, 10,  &results);
	if (err) {
		ERROR_MSG("mDNS query failed when searching MQTT server");
		vTaskDelete(NULL);
	}
	if (!results) {
		WARN_MSG("mDNS query failed to find any MQTT servers");
		vTaskDelete(NULL);
	}
	INFO_MSG("found mqtt server using mDNS: " IPSTR ":%u", IP2STR(&results->addr->addr.u_addr.ip4), results->port);

	/* connect to mqtt */
	char *host = NULL;
	asprintf(&host, IPSTR, IP2STR(&results->addr->addr.u_addr.ip4));
	esp_mqtt_client_config_t mqtt_cfg = { .host = host, .port = results->port };
	esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
	esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
	esp_mqtt_client_start(client);
	free(host);

	mdns_query_results_free(results);
#else
	/* static mqtt server defined */
	INFO_MSG("wifi connected, using hardcoded MQTT server: %s", MQTT_URI);
	esp_mqtt_client_config_t mqtt_cfg = { .uri = MQTT_URI };
	esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
	esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
	esp_mqtt_client_start(client);
#endif

	vTaskDelete(NULL);
}

#endif


#ifdef TARGET_ESP32
int app_main(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{
	/* low level init: initializes some system basics depending on platform */
	os_init();
	/* debug/log init */
	log_init();
	/* nvm initialization before wifi */
	nvm_init(NULL, 0);
	/* wifi base init */
	wifi_init();
	/* wifi configuration button */
	button_init(WIFI_CONFIG_BUTTON_GPIO);

#ifdef DO_MQTT_TEST
	/* start mqtt task */
	xTaskCreate(mqtt_task, "MQTT_TASK", 4096, NULL, 3, NULL);
#endif

	while (1) {
		os_wdt_reset();
		button_pressed(WIFI_CONFIG_BUTTON_GPIO, 1000) {
			button_repeat(WIFI_CONFIG_BUTTON_GPIO, 0);
			INFO_MSG("wifi configuration button pressed");
			wifi_smartconfig(true);
		}
		/* minimum delay for not to get watchdog timeout prints */
		os_delay_ms(10);
	}

	wifi_quit();
	nvm_quit();
	log_quit();
	os_quit();
	return 0;
}
