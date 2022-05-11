/*
 * Rotary encoder GPIO driver
 *
 * License: MIT
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#ifdef USE_GPIO

#include <libe/libe.h>
#include <math.h>


int8_t rot_enc_open(struct rot_enc *re, uint8_t a, uint8_t b)
{
	gpio_input(a);
	gpio_input(b);

	re->a = a;
	re->b = b;
	re->a_was = gpio_read(a);

	re->value = 0;
	re->min = NAN;
	re->max = NAN;
	re->step = 1.0;
	re->flags = 0;

	re->opt = rot_enc_opt;

	return 0;
}

int8_t rot_enc_rd_raw(struct rot_enc *re)
{
	uint8_t a = gpio_read(re->a);
	uint8_t b = gpio_read(re->b);
	if (re->a_was != a) {
		re->a_was = a;
		if ((a && !b) || (!a && b)) {
			return ROT_ENC_CCW;
		} else {
			return ROT_ENC_CW;
		}
	}
	return ROT_ENC_NOP;
}

float rot_enc_rd(struct rot_enc *re)
{
	uint8_t a = gpio_read(re->a);
	uint8_t b = gpio_read(re->b);
	if (re->a_was != a) {
		re->a_was = a;
		/* check for changes and apply HALF because we read both edges here that occur on one click */
		float step = re->step * 0.5;
		re->value += (a && !b) || (!a && b) ? -step : step;
		
		/* check for min and max, do both always since step can be positive or negative */
		if (re->value < re->min) {
			re->value = re->flags & ROT_ENC_FLAG_ROLL ? re->max : re->min;
		} else if (re->value > re->max) {
			re->value = re->flags & ROT_ENC_FLAG_ROLL ? re->min : re->max;
		}

		return re->value;
	}
	return re->value;
}

int8_t rot_enc_opt(struct rot_enc *re, uint8_t opt, void *value)
{
	switch (opt) {
	case ROT_ENC_OPT_GET_VALUE:
		*((float *)value) = re->value;
		return 0;
	case ROT_ENC_OPT_SET_VALUE:
		re->value = *((float *)value);
		return 0;
	case ROT_ENC_OPT_GET_MIN:
		*((float *)value) = re->min;
		return 0;
	case ROT_ENC_OPT_SET_MIN:
		re->min = *((float *)value);
		return 0;
	case ROT_ENC_OPT_GET_MAX:
		*((float *)value) = re->max;
		return 0;
	case ROT_ENC_OPT_SET_MAX:
		re->max = *((float *)value);
		return 0;
	case ROT_ENC_OPT_GET_STEP:
		*((float *)value) = re->step;
		return 0;
	case ROT_ENC_OPT_SET_STEP:
		re->step = *((float *)value);
		return 0;
	case ROT_ENC_OPT_GET_FLAGS:
		*((uint8_t *)value) = re->flags;
		return 0;
	case ROT_ENC_OPT_SET_FLAGS:
		re->flags = *((uint8_t *)value);
		return 0;
	}

	return -1;
}


#endif
