#include <avr/io.h>
#include <stdio.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#ifndef BAUD
#define BAUD 9600
#endif
#include <util/setbaud.h>

/* http://www.cs.mun.ca/~rod/Winter2007/4723/notes/serial/serial.html */

void uart0_init(void)
{
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;
#if USE_2X
    UCSR0A |= _BV(U2X0);
#else
    UCSR0A &= ~(_BV(U2X0));
#endif
    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */
    UCSR0B = _BV(RXEN0) | _BV(TXEN0);   /* Enable RX and TX */
}

void uart3_init(void)
{
    UBRR3H = UBRRH_VALUE;
    UBRR3L = UBRRL_VALUE;
#if USE_2X
    UCSR3A |= _BV(U2X3);
#else
    UCSR3A &= ~(_BV(U2X3));
#endif
    UCSR3C = _BV(UCSZ31) | _BV(UCSZ30); /* 8-bit data */
    UCSR3B = _BV(TXEN3);   /* Enable TX */
}

int uart0_putchar(char c, FILE *stream)
{
    (void) stream;

    if (c == '\n') {
        uart0_putchar('\r', stream);
    }

    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = c;
    return 0;
}

int uart3_putchar(char c, FILE *stream)
{
    (void) stream;

    if (c == '\n') {
        uart3_putchar('\r', stream);
    }

    loop_until_bit_is_set(UCSR3A, UDRE3);
    UDR3 = c;
    return 0;
}

int uart0_getchar(FILE *stream)
{
    (void) stream;
    loop_until_bit_is_set(UCSR0A, RXC0);
    return UDR0;
}

