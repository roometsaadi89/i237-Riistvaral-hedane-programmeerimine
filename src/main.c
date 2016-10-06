

#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"
#include "print_helper.h"
#include "hmi_msg.h"


#define BLINK_DELAY_MS 100

int main (void)
{
    /* set pin 3 of PORTA for output*/
    DDRA |= _BV(DDA3);
    /* Init error console as stderr in UART3 and print user code info */
    uart0_init();
    uart3_init();
    stdin = stdout = &uart0_io;
    stderr = &uart3_out;
    fprintf(stderr, "Version: %s built on: %s %s\n",
            GIT_DESCR, __DATE__, __TIME__);
    fprintf(stderr, "avr-libc version: %s\n", __AVR_LIBC_VERSION_STRING__);
    /*End UART3 init and info print */
    fprintf(stdout, STUD_NAME "\n");
    /* Print ASCII table*/
    print_ascii_tbl(stdout);
    unsigned char ascii[128] = {0};

    for (unsigned char i = 0; i < 128; i++) {
        ascii[i] = i;
    }

    print_for_human(stdout, ascii, 128)

    while (1) {
        /* set pin 3 high to turn led on */
        PORTA |= _BV(PORTA3);
        _delay_ms(BLINK_DELAY_MS);
        char inBuf = ' ';
        fprintf(stdout, "Enter month name first letter >");
        fscanf(stdin, "%c", &inBuf);
        fprintf(stdout, "%c\n", inBuf);

        for (int i = 0; i < 6, i++) {
            if (strncmp(&inBuf, nameMonth[i], 1)) {
                fprintf(stdout, "%s\n", nameMonth[i])
            }
        }

        /* set pin 3 low to turn led off */
        PORTA &= ~_BV(PORTA3);
        _delay_ms(BLINK_DELAY_MS);
    }
}
