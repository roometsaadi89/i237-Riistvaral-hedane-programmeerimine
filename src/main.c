#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include "hmi_msg.h"
#include "uart-wrapper.h"
#include "print_helper.h"
#include <avr/pgmspace.h>
#include "../lib/hd44780_111/hd44780.h"
#include "../lib/andygock_avr-uart/uart.h"
#include <avr/interrupt.h>

#define BAUD 9600
#define BLINK_DELAY_MS 100

volatile uint32_t seconds_elapsed;

static inline void init_system_clock(void)
{
    TCCR5A = 0; // Clear control register A
    TCCR5B = 0; // Clear control register B
    TCCR5B |= _BV(WGM52) | _BV(CS52); // CTC and fCPU/256
    OCR5A = 62549; // 1 s
    TIMSK5 |= _BV(OCIE5A); // Output Compare A Match Interrupt Enable
}
static inline void hw_init()
{
    /* set pin 3 of PORTA for output*/
    DDRA |= _BV(DDA3);
    /* Init stdio on UART0 and UART3 and print user code info */
    uart0_init(UART_BAUD_SELECT(BAUD, F_CPU));
    uart3_init(UART_BAUD_SELECT(BAUD, F_CPU));
    init_system_clock();
    sei();
    stdout = stdin = &uart0_io;
    stderr = &uart3_out;
    lcd_init();
    lcd_clrscr();
}

static inline void print_version()
{
    fprintf_P(stderr, VER_FW, PSTR(GIT_DESCR), PSTR(__DATE__), PSTR(__TIME__));
    fprintf_P(stderr, VER_LIBC, PSTR(__AVR_LIBC_VERSION_STRING__));
}

static inline void print_info()
{
    /* End stdio init and info print */
    fprintf_P(stdout, STUD_NAME);
    fputc('\n', stdout); /* Add a new line to the uart printout */
    lcd_puts_P(STUD_NAME);
    lcd_goto(0x40); /* Got to the beginning of the next line */
    /* ASCII table print */
    print_ascii_tbl(stdout);
    unsigned char ascii[128] = {0};

    for (unsigned char i = 0; i < sizeof(ascii); i++) {
        ascii[i] = i;
    }

    print_for_human(stdout, ascii, sizeof(ascii));
    fprintf_P(stdout, MONTH_MSG);
}
static inline void find_month()
{
    char month_first_letter;
    fscanf(stdin, "%c", &month_first_letter);
    fprintf(stdout, "%c\n", month_first_letter);

    for (int i = 0; i < 6; i++) {
        if (!strncmp_P(&month_first_letter, (PGM_P)pgm_read_word(&months[i]), 1)) {
            fprintf_P(stdout, (PGM_P)pgm_read_word(&months[i]));
            fputc('\n', stdout);
            lcd_puts_P((PGM_P)pgm_read_word(&months[i]));
            lcd_putc(' ');
        }
    }

    fprintf_P(stdout, MONTH_MSG);
    lcd_puts_P(PSTR("                "));
    lcd_goto(0x40);
}

static inline void heartbeat()
{
    static uint32_t time;
    uint32_t current_time = seconds_elapsed;

    if ((time - current_time) > 0) {
        //toggle led
        PORTA ^= _BV(PORTA3);
        fprintf_P(stderr, PSTR(UPTIME "\n"), current_time);
    }

    time = current_time;
}

int main (void)
{
    hw_init();
    print_version();
    print_info();

    while (1) {
        heartbeat();

        if (uart0_available()) {
            find_month();
        }
    }
}
/* System clock ISR */
ISR(TIMER5_COMPA_vect)
{
    seconds_elapsed++;
}
