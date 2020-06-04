/*
 * libe cross-platform library
 *
 * RC5 encryption.
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#include "rc5.h"

#define WORD_BITS       (sizeof(RC5_WORD) * 8)
#define WORDS_IN_KEY    (RC5_KEY_LEN / sizeof(RC5_WORD))

/* magic stuff */
#define P               0xb7e15163
#define Q               0x9e3779b9

/* TODO: make target specific assembly routines for these */
#define ROTL(x,y)       (((x) << ((y) & (WORD_BITS - 1))) | ((x) >> (WORD_BITS - ((y) & (WORD_BITS - 1)))))
#define ROTR(x,y)       (((x) >> ((y) & (WORD_BITS - 1))) | ((x) << (WORD_BITS - ((y) & (WORD_BITS - 1)))))


void rc5_init(rc5_context_t *rc5, void *key)
{
	RC5_WORD *S = rc5->S;
	RC5_WORD i, j, k, u = WORD_BITS / 8, A, B, L[WORDS_IN_KEY];
	uint8_t *K = key;

	for (i = RC5_KEY_LEN - 1, L[WORDS_IN_KEY - 1] = 0; i != -1; i--) {
		L[i / u] = (L[i / u] << 8) + K[i];
	}

	for (S[0] = P, i = 1; i < RC5_TABLE_SIZE; i++) {
		S[i] = S[i - 1] + Q;
	}

	for (A = B = i = j = k = 0; k < 3 * RC5_TABLE_SIZE; k++, i = (i + 1) % RC5_TABLE_SIZE, j = (j + 1) % WORDS_IN_KEY) {
		A = S[i] = ROTL(S[i] + (A + B), 3);
		B = L[j] = ROTL(L[j] + (A + B), (A + B));
	}
}

void rc5_encrypt(rc5_context_t *rc5, void *block)
{
	RC5_WORD *S = rc5->S;
	RC5_WORD *p = block;
	RC5_WORD i, A = p[0] + S[0], B = p[1] + S[1];
	for (i = 1; i <= RC5_ROUNDS; i++) {
		A = ROTL(A ^ B, B) + S[2 * i];
		B = ROTL(B ^ A, A) + S[2 * i + 1];
	}
	p[0] = A; p[1] = B;
}

void rc5_decrypt(rc5_context_t *rc5, void *block)
{
	RC5_WORD *S = rc5->S;
	RC5_WORD *p = block;
	RC5_WORD i, B = p[1], A = p[0];
	for (i = RC5_ROUNDS; i > 0; i--) {
		B = ROTR(B - S[2 * i + 1], A) ^ A;
		A = ROTR(A - S[2 * i], B) ^ B;
	}
	p[1] = B - S[1]; p[0] = A - S[0];
}



// typedef unsigned long WORD;       /* Should be 32-bit = 4 bytes */
// #define w        32             /* word size in bits                 */
// #define r        12             /* number of rounds                  */
// #define b        16             /* number of bytes in key            */
// #define c         4             /* number  words in key = ceil(8*b/w)*/
// #define t        26             /* size of table S = 2*(r+1) words   */
// WORD S[t];                      /* expanded key table                */
// WORD P = 0xb7e15163, Q = 0x9e3779b9;  /* magic constants             */
// /* Rotation operators. x must be unsigned, to get logical right shift*/
// #define ROTL(x,y) (((x)<<(y&(w-1))) | ((x)>>(w-(y&(w-1)))))
// #define ROTR(x,y) (((x)>>(y&(w-1))) | ((x)<<(w-(y&(w-1)))))

// void RC5_ENCRYPT(WORD *pt, WORD *ct) /* 2 WORD input pt/output ct    */
// {
// 	WORD i, A = pt[0] + S[0], B = pt[1] + S[1];
// 	for (i = 1; i <= r; i++) {
// 		A = ROTL(A ^ B, B) + S[2 * i];
// 		B = ROTL(B ^ A, A) + S[2 * i + 1];
// 	}
// 	ct[0] = A; ct[1] = B;
// }

// void RC5_DECRYPT(WORD *ct, WORD *pt) /* 2 WORD input ct/output pt    */
// {
// 	WORD i, B = ct[1], A = ct[0];
// 	for (i = r; i > 0; i--) {
// 		B = ROTR(B - S[2 * i + 1], A)^A;
// 		A = ROTR(A - S[2 * i], B)^B;
// 	}
// 	pt[1] = B - S[1]; pt[0] = A - S[0];
// }

// void RC5_SETUP(unsigned char *K) /* secret input key K[0...b-1]      */
// {
// 	WORD i, j, k, u = w / 8, A, B, L[c];
// 	/* Initialize L, then S, then mix key into S */
// 	for (i = b - 1, L[c - 1] = 0; i != -1; i--) {
// 		L[i / u] = (L[i / u] << 8) + K[i];
// 	}
// 	for (S[0] = P, i = 1; i < t; i++) {
// 		S[i] = S[i - 1] + Q;
// 	}
// 	for (A = B = i = j = k = 0; k < 3 * t; k++, i = (i + 1) % t, j = (j + 1) % c) { /* 3*t > 3*c */
// 		A = S[i] = ROTL(S[i] + (A + B), 3);
// 		B = L[j] = ROTL(L[j] + (A + B), (A + B));
// 	}
// }