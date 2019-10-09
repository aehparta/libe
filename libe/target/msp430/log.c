/*
 * libe cross-platform library: Logging
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#include <stdarg.h>
#include <libe/os.h>
#include <libe/log.h>
#include <libe/log.h>


#ifndef MSP430_LOG_TX_PSEL
#define MSP430_LOG_TX_PSEL P3SEL
#endif
#ifndef MSP430_LOG_TX_PIN
#define MSP430_LOG_TX_PIN 4
#endif

// static int printchar(char c, FILE *stream)
// {
// 	return 0;
// }

int log_init(void)
{
	/* calibrate peripheral clock to 1 MHz */
	DCOCTL = 0;
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;

	MSP430_LOG_TX_PSEL |= (1 << MSP430_LOG_TX_PIN);

	UCA0CTL1 |= UCSSEL_2; // SMCLK
	UCA0BR0 = 0x08; // 1MHz 115200
	UCA0BR1 = 0x00; // 1MHz 115200
	UCA0MCTL = UCBRS2 + UCBRS0; // Modulation UCBRSx = 5
	UCA0CTL1 &= ~UCSWRST; // **Initialize USCI state machine**
	UC0IE |= UCA0RXIE; // Enable USCI_A0 RX interrupt

	while (1) {
		while (UCA1TXIFG);
		UCA0TXBUF = 'A';
	}


	return 0;
}

void log_quit(void)
{
}

void log_msg(int level, const char *file, int line, const char *func, const char *msg, ...)
{
	if (level == LOG_LEVEL_DEBUG) {
		LOG_PRINTF(LDC_PURPLE "D:" LDC_DEFAULT);
	} else if (level == LOG_LEVEL_INFO) {
		LOG_PRINTF(LDC_BLUE "I:" LDC_DEFAULT);
	} else if (level == LOG_LEVEL_NOTICE) {
		LOG_PRINTF(LDC_CYAN "N:" LDC_DEFAULT);
	} else if (level == LOG_LEVEL_WARN) {
		LOG_PRINTF(LDC_YELLOW "W:" LDC_DEFAULT);
	} else if (level == LOG_LEVEL_ERR) {
		LOG_PRINTF(LDC_RED "E:" LDC_DEFAULT);
	} else if (level == LOG_LEVEL_CRIT) {
		LOG_PRINTF(LDC_REDB "C:" LDC_DEFAULT);
	}

	va_list args;
	va_start(args, msg);
	vfprintf(stdout, msg, args);
	va_end(args);
	LOG_PRINTF("\r\n");
}
