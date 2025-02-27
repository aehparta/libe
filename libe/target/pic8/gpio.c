/*
 * pic8 specific gpio.
 */

#ifdef USE_GPIO

#include <libe/libe.h>


#ifdef GPIO_HAS_PPS
void gpio_pps(uint8_t pin, uint8_t action)
{
	switch (pin) {
	/* port A */
#ifdef RA0PPS
	case 0:
		RA0PPS = action;
		break;
#endif
#ifdef RA1PPS
	case 1:
		RA1PPS = action;
		break;
#endif
#ifdef RA2PPS
	case 2:
		RA2PPS = action;
		break;
#endif
#ifdef RA3PPS
	case 3:
		RA3PPS = action;
		break;
#endif
#ifdef RA4PPS
	case 4:
		RA4PPS = action;
		break;
#endif
#ifdef RA5PPS
	case 5:
		RA5PPS = action;
		break;
#endif
#ifdef RA6PPS
	case 6:
		RA6PPS = action;
		break;
#endif
#ifdef RA7PPS
	case 7:
		RA7PPS = action;
		break;
#endif
	/* port B */
#ifdef RB0PPS
	case 8:
		RB0PPS = action;
		break;
#endif
#ifdef RB1PPS
	case 9:
		RB1PPS = action;
		break;
#endif
#ifdef RB2PPS
	case 10:
		RB2PPS = action;
		break;
#endif
#ifdef RB3PPS
	case 11:
		RB3PPS = action;
		break;
#endif
#ifdef RB4PPS
	case 12:
		RB4PPS = action;
		break;
#endif
#ifdef RB5PPS
	case 13:
		RB5PPS = action;
		break;
#endif
#ifdef RB6PPS
	case 14:
		RB6PPS = action;
		break;
#endif
#ifdef RB7PPS
	case 15:
		RB7PPS = action;
		break;
#endif
	/* port C */
#ifdef RC0PPS
	case 16:
		RC0PPS = action;
		break;
#endif
#ifdef RC1PPS
	case 17:
		RC1PPS = action;
		break;
#endif
#ifdef RC2PPS
	case 18:
		RC2PPS = action;
		break;
#endif
#ifdef RC3PPS
	case 19:
		RC3PPS = action;
		break;
#endif
#ifdef RC4PPS
	case 20:
		RC4PPS = action;
		break;
#endif
#ifdef RC5PPS
	case 21:
		RC5PPS = action;
		break;
#endif
#ifdef RC6PPS
	case 22:
		RC6PPS = action;
		break;
#endif
#ifdef RC7PPS
	case 23:
		RC7PPS = action;
		break;
#endif
	/* port D */
#ifdef RD0PPS
	case 24:
		RD0PPS = action;
		break;
#endif
#ifdef RD1PPS
	case 25:
		RD1PPS = action;
		break;
#endif
#ifdef RD2PPS
	case 26:
		RD2PPS = action;
		break;
#endif
#ifdef RD3PPS
	case 27:
		RD3PPS = action;
		break;
#endif
#ifdef RD4PPS
	case 28:
		RD4PPS = action;
		break;
#endif
#ifdef RD5PPS
	case 29:
		RD5PPS = action;
		break;
#endif
#ifdef RD6PPS
	case 30:
		RD6PPS = action;
		break;
#endif
#ifdef RD7PPS
	case 31:
		RD7PPS = action;
		break;
#endif
	/* port E */
#ifdef RE0PPS
	case 32:
		RE0PPS = action;
		break;
#endif
#ifdef RE1PPS
	case 33:
		RE1PPS = action;
		break;
#endif
#ifdef RE2PPS
	case 34:
		RE2PPS = action;
		break;
#endif
#ifdef RE3PPS
	case 35:
		RE3PPS = action;
		break;
#endif
#ifdef RE4PPS
	case 36:
		RE4PPS = action;
		break;
#endif
#ifdef RE5PPS
	case 37:
		RE5PPS = action;
		break;
#endif
#ifdef RE6PPS
	case 38:
		RE6PPS = action;
		break;
#endif
#ifdef RE7PPS
	case 39:
		RE7PPS = action;
		break;
#endif
	/* port F */
#ifdef RF0PPS
	case 40:
		RF0PPS = action;
		break;
#endif
#ifdef RF1PPS
	case 41:
		RF1PPS = action;
		break;
#endif
#ifdef RF2PPS
	case 42:
		RF2PPS = action;
		break;
#endif
#ifdef RF3PPS
	case 43:
		RF3PPS = action;
		break;
#endif
#ifdef RF4PPS
	case 44:
		RF4PPS = action;
		break;
#endif
#ifdef RF5PPS
	case 45:
		RF5PPS = action;
		break;
#endif
#ifdef RF6PPS
	case 46:
		RF6PPS = action;
		break;
#endif
#ifdef RF7PPS
	case 47:
		RF7PPS = action;
		break;
#endif
	}
}
#endif

#endif
