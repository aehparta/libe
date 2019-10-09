/*
 * libe cross-platform library: Logging
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#ifdef USE_LOG

#include <stdio.h>
#include <stdarg.h>
#include <esp_log.h>
#include <libe/libe.h>

#ifndef ESP32_LOG_TAG
#define ESP32_LOG_TAG   "libe"
#endif

int log_init(void)
{
	return 0;
}

void log_quit(void)
{
}

void log_msg(int level, const char *file, int line, const char *func, const char *msg, ...)
{
	char *s = NULL;
	va_list args;
	va_start(args, msg);
	vasprintf(&s, msg, args);
	va_end(args);

	if (level == LOG_LEVEL_DEBUG) {
		ESP_LOGD(ESP32_LOG_TAG, "%s", s);
	} else if (level == LOG_LEVEL_INFO) {
		ESP_LOGI(ESP32_LOG_TAG, "%s", s);
	} else if (level == LOG_LEVEL_NOTICE) {
		ESP_LOGI(ESP32_LOG_TAG, "%s", s);
	} else if (level == LOG_LEVEL_WARN) {
		ESP_LOGW(ESP32_LOG_TAG, "%s", s);
	} else if (level == LOG_LEVEL_ERR) {
		ESP_LOGE(ESP32_LOG_TAG, "%s", s);
	} else if (level == LOG_LEVEL_CRIT) {
		ESP_LOGE(ESP32_LOG_TAG, "%s", s);
	}

	free(s);
}

#endif
