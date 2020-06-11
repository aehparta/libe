/*
 * Non-volatile memory handler for esp32.
 */

#include <nvs_flash.h>
#include <libe/log.h>
#include <libe/nvm.h>


nvs_handle esp32_nvm_handle;


int nvm_init(void *context, size_t size)
{
	if (!context) {
		context = "default";
	}
	esp_err_t err = nvs_flash_init();
	if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
		ERROR_IF_R(nvs_flash_erase(), -1, "nvm erase flash failed");
		err = nvs_flash_init();
	}
	ERROR_IF_R(err, -1, "nvm init failed");
	ERROR_IF_R(nvs_open(context, NVS_READWRITE, &esp32_nvm_handle), -1, "nvm open failed");

	return 0;
}

void nvm_quit(void)
{
	nvs_close(esp32_nvm_handle);
}

void nvm_erase(void)
{
	nvs_flash_erase();
	nvs_flash_init();
}

uint8_t nvm_read_byte(void *address, uint8_t default_value)
{
	uint8_t value;
	char key[16];
	snprintf(key, sizeof(key), "%p", address);
	if (nvs_get_u8(esp32_nvm_handle, key, &value)) {
		return default_value;
	}
	return value;
}

void nvm_write_byte(void *address, uint8_t value)
{
	char key[16];
	snprintf(key, sizeof(key), "%p", address);
	ERROR_IF(nvs_set_u8(esp32_nvm_handle, key, value), "nvm_write_byte() failed");
}

uint16_t nvm_read_word(void *address, uint16_t default_value)
{
	uint16_t value;
	char key[16];
	snprintf(key, sizeof(key), "%p", address);
	if (nvs_get_u16(esp32_nvm_handle, key, &value)) {
		return default_value;
	}
	return value;
}

void nvm_write_word(void *address, uint16_t value)
{
	char key[16];
	snprintf(key, sizeof(key), "%p", address);
	ERROR_IF(nvs_set_u16(esp32_nvm_handle, key, value), "nvm_write_word() failed");
}

uint32_t nvm_read_dword(void *address, uint32_t default_value)
{
	uint32_t value;
	char key[16];
	snprintf(key, sizeof(key), "%p", address);
	if (nvs_get_u32(esp32_nvm_handle, key, &value)) {
		return default_value;
	}
	return value;
}

void nvm_write_dword(void *address, uint32_t value)
{
	char key[16];
	snprintf(key, sizeof(key), "%p", address);
	ERROR_IF(nvs_set_u32(esp32_nvm_handle, key, value), "nvm_write_dword() failed");
}

float nvm_read_float(void *address, float default_value)
{
	float value;
	char key[16];
	size_t len = sizeof(value);
	snprintf(key, sizeof(key), "%p", address);
	if (nvs_get_blob(esp32_nvm_handle, key, &value, &len)) {
		return default_value;
	}
	return value;
}

void nvm_write_float(void *address, float value)
{
	char key[16];
	snprintf(key, sizeof(key), "%p", address);
	ERROR_IF(nvs_set_blob(esp32_nvm_handle, key, &value, sizeof(value)), "nvm_write_float() failed");
}
