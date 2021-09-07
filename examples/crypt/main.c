/*
 * Examples and tests for encryption.
 */

#include <stdio.h>
#include <string.h>
#include <openssl/aes.h>
#include <libe/libe.h>

int aes128_do(uint8_t key[16])
{
	int err = 0;
	aes_128_context_t ctx;
	aes_128_init(&ctx, key);

	printf(LDC_YELLOWB "AES 128\n");
	printf(LDC_BLUEB "  Using key: %.16s\n", key);

	/* aes block size must be multiples of 16 bytes */
	uint8_t data[2][33] = {"abcdefgh12345678\0", "abcdefgh12345678dataDATAmoreMORE\0"};

	/* using our lib */
	for (int i = 0; i < 2; i++) {
		size_t size = strlen((char *)data[i]);
		uint8_t *p = malloc(size);
		memcpy(p, data[i], size);

		printf(LDC_CYANB "  Original data: %s (size: %lu)\n", data[i], size);

		for (size_t j = 0; j < size; j += 16) {
			aes_128_encrypt(&ctx, p + j);
		}

		printf("  Encrypted HEX: ");
		HEX_DUMP(p, size, 0);
		printf("          ASCII: ");
		ASCII_DUMP(p, size, 0);

		for (size_t j = 0; j < size; j += 16) {
			aes_128_decrypt(&ctx, p + j);
		}

		printf("  Decrypted:     ");
		ASCII_DUMP(p, size, 0);

		if (memcmp(p, data[i], size) == 0) {
			printf(LDC_GREENB "  OK\n");
		} else {
			printf(LDC_REDB "  FAILED\n");
			err++;
		}

		free(p);
	}

	/* test against openssl aes implementation */
	AES_KEY akey;

	/* using openssl to encrypt */
	printf(LDC_BLUEB "  Using openssl for encryption and libe for decryption\n");
	AES_set_encrypt_key(key, 128, &akey);
	for (int i = 0; i < 2; i++) {
		size_t size = strlen((char *)data[i]);
		uint8_t *p = malloc(size);
		memcpy(p, data[i], size);

		printf(LDC_CYANB "  Original data: %s (size: %lu)\n", data[i], size);

		for (size_t j = 0; j < size; j += 16) {
			AES_encrypt(p + j, p + j, &akey);
		}

		printf("  Encrypted HEX: ");
		HEX_DUMP(p, size, 0);
		printf("          ASCII: ");
		ASCII_DUMP(p, size, 0);

		for (size_t j = 0; j < size; j += 16) {
			aes_128_decrypt(&ctx, p + j);
		}

		printf("  Decrypted:     ");
		ASCII_DUMP(p, size, 0);

		if (memcmp(p, data[i], size) == 0) {
			printf(LDC_GREENB "  OK\n");
		} else {
			printf(LDC_REDB "  FAILED\n");
			err++;
		}

		free(p);
	}

	/* using openssl to decrypt */
	printf(LDC_BLUEB "  Using libe for encryption and openssl for decryption\n");
	AES_set_decrypt_key(key, 128, &akey);
	for (int i = 0; i < 2; i++) {
		size_t size = strlen((char *)data[i]);
		uint8_t *p = malloc(size);
		memcpy(p, data[i], size);

		printf(LDC_CYANB "  Original data: %s (size: %lu)\n", data[i], size);

		for (size_t j = 0; j < size; j += 16) {
			aes_128_encrypt(&ctx, p + j);
		}

		printf("  Encrypted HEX: ");
		HEX_DUMP(p, size, 0);
		printf("          ASCII: ");
		ASCII_DUMP(p, size, 0);

		for (size_t j = 0; j < size; j += 16) {
			AES_decrypt(p + j, p + j, &akey);
		}

		printf("  Decrypted:     ");
		ASCII_DUMP(p, size, 0);

		if (memcmp(p, data[i], size) == 0) {
			printf(LDC_GREENB "  OK\n");
		} else {
			printf(LDC_REDB "  FAILED\n");
			err++;
		}

		free(p);
	}

	printf(LDC_DEFAULT);
	return err;
}

int aes256_do(uint8_t key[32])
{
	int err = 0;
	aes_256_context_t ctx;
	aes_256_init(&ctx, key);

	printf(LDC_YELLOWB "AES 256\n");
	printf(LDC_BLUEB "  Using key: %.32s\n", key);

	/* aes block size must be multiples of 16 bytes */
	uint8_t data[2][33] = {"abcdefgh12345678\0", "abcdefgh12345678dataDATAmoreMORE\0"};

	/* using our lib */
	for (int i = 0; i < 2; i++) {
		size_t size = strlen((char *)data[i]);
		uint8_t *p = malloc(size);
		memcpy(p, data[i], size);

		printf(LDC_CYANB "  Original data: %s (size: %lu)\n", data[i], size);

		for (size_t j = 0; j < size; j += 16) {
			aes_256_encrypt(&ctx, p + j);
		}

		printf("  Encrypted HEX: ");
		HEX_DUMP(p, size, 0);
		printf("          ASCII: ");
		ASCII_DUMP(p, size, 0);

		for (size_t j = 0; j < size; j += 16) {
			aes_256_decrypt(&ctx, p + j);
		}

		printf("  Decrypted:     ");
		ASCII_DUMP(p, size, 0);

		if (memcmp(p, data[i], size) == 0) {
			printf(LDC_GREENB "  OK\n");
		} else {
			printf(LDC_REDB "  FAILED\n");
			err++;
		}

		free(p);
	}

	/* test against openssl aes implementation */
	AES_KEY akey;

	/* using openssl to encrypt */
	printf(LDC_BLUEB "  Using openssl for encryption and libe for decryption\n");
	AES_set_encrypt_key(key, 256, &akey);
	for (int i = 0; i < 2; i++) {
		size_t size = strlen((char *)data[i]);
		uint8_t *p = malloc(size);
		memcpy(p, data[i], size);

		printf(LDC_CYANB "  Original data: %s (size: %lu)\n", data[i], size);

		for (size_t j = 0; j < size; j += 16) {
			AES_encrypt(p + j, p + j, &akey);
		}

		printf("  Encrypted HEX: ");
		HEX_DUMP(p, size, 0);
		printf("          ASCII: ");
		ASCII_DUMP(p, size, 0);

		for (size_t j = 0; j < size; j += 16) {
			aes_256_decrypt(&ctx, p + j);
		}

		printf("  Decrypted:     ");
		ASCII_DUMP(p, size, 0);

		if (memcmp(p, data[i], size) == 0) {
			printf(LDC_GREENB "  OK\n");
		} else {
			printf(LDC_REDB "  FAILED\n");
			err++;
		}

		free(p);
	}

	/* using openssl to decrypt */
	printf(LDC_BLUEB "  Using libe for encryption and openssl for decryption\n");
	AES_set_decrypt_key(key, 256, &akey);
	for (int i = 0; i < 2; i++) {
		size_t size = strlen((char *)data[i]);
		uint8_t *p = malloc(size);
		memcpy(p, data[i], size);

		printf(LDC_CYANB "  Original data: %s (size: %lu)\n", data[i], size);

		for (size_t j = 0; j < size; j += 16) {
			aes_256_encrypt(&ctx, p + j);
		}

		printf("  Encrypted HEX: ");
		HEX_DUMP(p, size, 0);
		printf("          ASCII: ");
		ASCII_DUMP(p, size, 0);

		for (size_t j = 0; j < size; j += 16) {
			AES_decrypt(p + j, p + j, &akey);
		}

		printf("  Decrypted:     ");
		ASCII_DUMP(p, size, 0);

		if (memcmp(p, data[i], size) == 0) {
			printf(LDC_GREENB "  OK\n");
		} else {
			printf(LDC_REDB "  FAILED\n");
			err++;
		}

		free(p);
	}

	printf(LDC_DEFAULT);
	return err;
}

int rc5_do(uint8_t key[16])
{
	int err = 0;

	printf(LDC_YELLOWB "RC5-32/12/16\n");
	printf(LDC_BLUEB "  Using key: %.16s\n", key);

	/* rc5 block size used by libe is 8 bytes */
	uint8_t data[2][17] = {"dataDATA\0", "dataDATAmoreMORE\0"};

	/* setup rc5 context */
	rc5_context_t rc5;
	rc5_init(&rc5, key);

	/* using our lib */
	for (int i = 0; i < 2; i++) {
		size_t size = strlen((char *)data[i]);
		uint8_t *p = malloc(size);
		memcpy(p, data[i], size);

		printf(LDC_CYANB "  Original data: %s (size: %lu)\n", data[i], size);

		for (size_t j = 0; j < size; j += 8) {
			rc5_encrypt(&rc5, p + j);
		}

		printf("  Encrypted HEX: ");
		HEX_DUMP(p, size, 0);
		printf("          ASCII: ");
		ASCII_DUMP(p, size, 0);

		for (size_t j = 0; j < size; j += 8) {
			rc5_decrypt(&rc5, p + j);
		}

		printf("  Decrypted:     ");
		ASCII_DUMP(p, size, 0);

		if (memcmp(p, data[i], size) == 0) {
			printf(LDC_GREENB "  OK\n");
		} else {
			printf(LDC_REDB "  FAILED\n");
			err++;
		}

		free(p);
	}

	printf(LDC_DEFAULT);
	return 0;
}

int xtea_do(uint8_t key[16])
{
	int err = 0;

	printf(LDC_YELLOWB "XTEA\n");
	printf(LDC_BLUEB "  Using key: %.16s\n", key);

	/* xtea block size must be multiples of 8 bytes */
	uint8_t data[2][17] = {"dataDATA\0", "dataDATAmoreMORE\0"};

	/* using our lib */
	for (int i = 0; i < 2; i++) {
		size_t size = strlen((char *)data[i]);
		uint8_t *p = malloc(size);
		memcpy(p, data[i], size);

		printf(LDC_CYANB "  Original data: %s (size: %lu)\n", data[i], size);

		for (size_t j = 0; j < size; j += 8) {
			xtea_encrypt(p + j, key);
		}

		printf("  Encrypted HEX: ");
		HEX_DUMP(p, size, 0);
		printf("          ASCII: ");
		ASCII_DUMP(p, size, 0);

		for (size_t j = 0; j < size; j += 8) {
			xtea_decrypt(p + j, key);
		}

		printf("  Decrypted:     ");
		ASCII_DUMP(p, size, 0);

		if (memcmp(p, data[i], size) == 0) {
			printf(LDC_GREENB "  OK\n");
		} else {
			printf(LDC_REDB "  FAILED\n");
			err++;
		}

		free(p);
	}

	printf(LDC_DEFAULT);
	return 0;
}

int xxtea_do(uint8_t key[16])
{
	int err = 0;

	printf(LDC_YELLOWB "XXTEA\n");
	printf(LDC_BLUEB "  Using key: %.16s\n", key);

	/* xxtea block size must be multiples of 4 bytes and at least 8 bytes */
	uint8_t data[3][17] = {"dataDATA\0", "dataDATA1234\0", "dataDATAmoreMORE\0"};

	/* using our lib */
	for (int i = 0; i < 3; i++) {
		size_t size = strlen((char *)data[i]);
		uint8_t *p = malloc(size);
		memcpy(p, data[i], size);

		printf(LDC_CYANB "  Original data: %s (size: %lu)\n", data[i], size);

		xxtea_encrypt(p, size, key);

		printf("  Encrypted HEX: ");
		HEX_DUMP(p, size, 0);
		printf("          ASCII: ");
		ASCII_DUMP(p, size, 0);

		xxtea_decrypt(p, size, key);

		printf("  Decrypted:     ");
		ASCII_DUMP(p, size, 0);

		if (memcmp(p, data[i], size) == 0) {
			printf(LDC_GREENB "  OK\n");
		} else {
			printf(LDC_REDB "  FAILED\n");
			err++;
		}

		free(p);
	}

	printf(LDC_DEFAULT);
	return 0;
}

int main(int argc, char *argv[])
{
	/* generate random ascii key if not given from command line */
	uint8_t key[32];
	if (argc >= 2) {
		memset(key, 0, sizeof(key));
		strncpy((char *)key, argv[1], sizeof(key));
	} else {
		srand(time(NULL));
		for (int i = 0; i < sizeof(key); i++) {
			do {
				key[i] = (uint8_t)rand();
			} while (!isalnum(key[i]));
		}
	}

	/* base init */
	os_init();
	log_init();

	/* different encryption algorithms */
	int err = 0;
	err += aes128_do(key);
	err += aes256_do(key);
	err += rc5_do(key);
	err += xtea_do(key);
	err += xxtea_do(key);

	return err;
}
