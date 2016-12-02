#include <avr/io.h>
#include <stdio.h>
#include "../lib/andygock_avr-uart/uart.h"

int uart0_putchar(char c, FILE *stream)
{
    (void) stream;

    if (c == '\n') {
        uart0_putchar('\r', stream);
    }

    uart0_putc(c);
    return 0;
}

int uart0_getchar(FILE *stream)
{
    (void) stream;
    return uart0_getc() & 0x00FF;
}

int uart3_putchar(char c, FILE *stream)
{
    (void) stream;

    if (c == '\n') {
        uart3_putchar('\r', stream);
    }

    uart3_putc(c);
    return 0;
}
