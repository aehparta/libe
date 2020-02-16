/*
 * Rotary encoder GPIO driver
 *
 * TODO: need to be optmized for smaller platforms.
 * pins (a and b in the struct) used from variables are slow on AVR and so on small platforms.
 * 
 * License: MIT
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#ifndef _DRIVERS_GPIO_ROT_ENC_H_
#define _DRIVERS_GPIO_ROT_ENC_H_

#ifdef __cplusplus
extern "C" {
#endif

#define ROT_ENC_CCW             (-1)
#define ROT_ENC_NOP             0
#define ROT_ENC_CW              1

#define ROT_ENC_FLAG_ROLL       0x01

enum {
	ROT_ENC_OPT_GET_VALUE = OPT_START_DEVICE,
	ROT_ENC_OPT_SET_VALUE,
	ROT_ENC_OPT_GET_MIN,
	ROT_ENC_OPT_SET_MIN,
	ROT_ENC_OPT_GET_MAX,
	ROT_ENC_OPT_SET_MAX,
	ROT_ENC_OPT_GET_STEP,
	ROT_ENC_OPT_SET_STEP,
	ROT_ENC_OPT_GET_FLAGS,
	ROT_ENC_OPT_SET_FLAGS,
};

struct rot_enc {
	uint8_t a;
	uint8_t b;
	uint8_t a_was;

	int8_t (*opt)(struct rot_enc *re, uint8_t opt, void *value);

	float value;
	float min;
	float max;
	float step;
	int8_t flags;
};

int8_t rot_enc_open(struct rot_enc *re, uint8_t a, uint8_t b);
int8_t rot_enc_rd_raw(struct rot_enc *re);
float rot_enc_rd(struct rot_enc *re);

int8_t rot_enc_opt(struct rot_enc *re, uint8_t opt, void *value);

#ifdef __cplusplus
}
#endif

#endif /* _DRIVERS_GPIO_ROT_ENC_H_ */
