/*
 * Random testing area for development.
 */

#include <string.h>
#include <libe/libe.h>


struct optt {
	int8_t (*opt)(struct optt *optt, uint8_t opt, void *value, uint8_t flags);
};

int8_t o(struct optt *optt, uint8_t opt, void *value, uint8_t flags)
{
	printf("%s opt %u\n", (flags & OPT_FLAG_MASK_SET_GET) == OPT_FLAG_GET ? "get" : "set", opt);
	return -1;
}


#ifdef TARGET_ESP32
int app_main(int argc, char *argv[])
#else
int main(void)
#endif
{
	struct optt x;
	float f = 32;

	memset(&x, 0, sizeof(x));

	opt_set(&x, 8, &f);

	x.opt = o;

	opt_set(&x, 9, &f);
	opt_set_u8(&x, 10, 1);

	opt_get(&x, 17, &f);
	
	return 0;
}
