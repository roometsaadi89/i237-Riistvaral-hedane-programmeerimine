/*This file is part of Foobar.

    Foobar is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>. */

#include <avr/io.h>
#include <stdio.h>
#include "hmi_msg.h"
#include "../lib/andygock_avr-uart/uart.h"


int uart0_putchar(char c, FILE *stream)
{
    (void) stream;

    if (c == '\n') {
        uart0_putc('\r');
        uart0_putc('\n');
        return 0;
    }

    uart0_putc((uint8_t)c);
    return 0;
}


int uart3_putchar(char c, FILE *stream)
{
    (void) stream;

    if (c == '\n') {
        uart3_putc('\r');
        uart3_putc('\n');
        return 0;
    }

    uart3_putc((uint8_t)c);
    return 0;
}


int uart0_getchar(FILE *stream)
{
    (void) stream;
    return (uart0_getc() & UART_STATUS_MASK);
}

