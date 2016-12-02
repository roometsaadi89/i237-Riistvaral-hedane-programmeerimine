#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include "hmi_msg.h"
#include "uart-wrapper.h"
#include "print_helper.h"
#include "../lib/hd44780_111/hd44780.h"
#include "../lib/andygock_avr-uart/uart.h"
#include "../lib/helius_microrl/microrl.h"
#include "cli_microrl.h"

volatile uint32_t seconds;

static microrl_t rl;
static microrl_t *prl = &rl;


static inline void init_clock(void)
{
    TCCR5A = 0;
    TCCR5B = 0;
    TCCR5B |= _BV(WGM52) | _BV(CS52);
    OCR5A = 62549;
    TIMSK5 |= _BV(OCIE5A);
}


static inline void init_hw (void)
{
    DDRA |= _BV(DDA3);
    init_clock();
    uart0_init(UART_BAUD_SELECT(9600, F_CPU));
    uart3_init(UART_BAUD_SELECT(9600, F_CPU));
    stdout = stdin = &uart0_io;
    stderr = &uart3_out;
    lcd_init();
    lcd_clrscr();
    sei();
}


static inline void print_info (void)
{
    print_versions(stderr);
    fprintf_P(stdout, PSTR(STUD_NAME));
    fputc('\n', stdout);
    lcd_puts_P(PSTR(STUD_NAME));
}


static inline void cli_init (void)
{
    microrl_init (prl, cli_print);
    microrl_set_execute_callback (prl, cli_execute);
}


static inline void heartbeat (void)
{
    static uint32_t time;
    uint32_t elapsed = 0;
    ATOMIC_BLOCK(ATOMIC_FORCEON) {
        elapsed = seconds;
    }

    if ((time - elapsed) > 0) {
        PORTA ^= _BV(PORTA3);
        fprintf_P(stderr, PSTR(UPTIME "\n"), elapsed);
    }

    time = elapsed;
}


int main (void)
{
    init_hw();
    print_info();
    cli_init();

    while (1) {
        heartbeat();
        microrl_insert_char (prl, cli_get_char());
    }
}


ISR(TIMER5_COMPA_vect)
{
    seconds++;
}
