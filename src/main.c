
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/atomic.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>

#include "rfid.h"
#include "hmi_msg.h"
#include "../lib/andygock_avr-uart/uart.h"
#include "uart-wrapper.h"
#include "print_helper.h"
#include "../lib/hd44780_111/hd44780.h"
#include "../lib/helius_microrl/microrl.h"
#include "cli_microrl.h"
#include "../lib/matejx_avr_lib/mfrc522.h"

#define BLINK_DELAY_MS 100
#define BAUD 9600


volatile uint32_t seconds;

/* Variable that shows if the last card touched was in list (a legit one) */
int in_list;

/* Variable that shows if the actions dealt with card touched are finished or not (LED and LCD operations) */
int done;

/** Create microrl object and pointer on it **/
microrl_t rl;
microrl_t *prl = &rl;


static inline void init_counter(void)
{
    /* Reset counter */
    seconds = 0;
    TCCR1A = 0;
    TCCR1B = 0;
    /* Turn on CTC (Clear Timer on Compare) */
    TCCR1B |= _BV(WGM12);
    /* fCPU/256 */
    TCCR1B |= _BV(CS12);
    /* Count seconds */
    OCR1A = 62549;
    /* Output Compare A Match Interrupt Enable */
    TIMSK1 |= _BV(OCIE1A);
}


static inline void init_rfid_reader(void)
{
    /* Init RFID-RC522 */
    MFRC522_init();
    PCD_Init();
}


static inline void init_io(void)
{
    /* set pin 3 of PORTA for output*/
    DDRA |= _BV(DDA3);
    DDRA |= _BV(DDA1);
    /* Init error console as stderr in UART3 and standard I/O
     * as stdin and stdout in UART0. */
    uart3_init(UART_BAUD_SELECT(BAUD, F_CPU));
    uart0_init(UART_BAUD_SELECT(BAUD, F_CPU));
    stderr = &uart3_out;
    stdin = stdout = &uart0_io;
    /* Enable all interrupts */
    sei();
}


static inline void init_microrl(void)
{
    /** call init with ptr to microrl instance and print callback **/
    microrl_init(prl, print);
    /** set callback for execute **/
    microrl_set_execute_callback (prl, cli_execute);
}


static inline void print_info(void)
{
    /* Print user info in stderr*/
    fprintf_P(stderr, PSTR(PRO1 GIT_DESCR PRO2 __DATE__ " " __TIME__ "\n" ));
    fprintf_P(stderr, PSTR(LIBC __AVR_LIBC_VERSION_STRING__ " " AVR __VERSION__
                           "\n"));
    /* Print STUD_NAME in stdout*/
    fprintf_P(stdout, PSTR(STUD_NAME "\n"));
}


static inline void heartbeat(void)
{
    static uint32_t last_time;
    uint32_t current_time;
    ATOMIC_BLOCK(ATOMIC_FORCEON) {
        current_time = seconds;
    }

    if ((last_time - current_time) > 0) {
        fprintf_P(stderr, PSTR(UPTIME "\n"), seconds);
        PORTA ^= _BV(PORTA3);
    }

    last_time = current_time;
}


/* Checks, if RFID chip is in contact and if it is a legit one */
void check(void)
{
    /* Last time a legit chip was in contact */
    static uint32_t since_allowed;
    /* Last time an unknown chip was in contact*/
    static uint32_t since_blocked;

    /* In case of a contact */
    if (PICC_IsNewCardPresent()) {
        /* Kind of a same as in_list, but with smaller scope (needed, when illegit card is in contact while door is open */
        int found = 0;

        /* In case we have cards in list */
        if (head != NULL) {
            card_t *current;
            current = head;

            /* Iteration through the list */
            while (current != NULL) {
                Uid card_uid;
                Uid *uid_ptra = &card_uid;
                PICC_ReadCardSerial(uid_ptra);

                /* In case the card in list examined is the same in contact. */
                if (!memcmp(card_uid.uidByte, current->uid, card_uid.size)) {
                    in_list = 1;
                    PORTA |= _BV(PORTA1);
                    since_allowed = seconds;
                    lcd_clrscr();
                    lcd_puts_P(PSTR(STUD_NAME));
                    lcd_goto(0x40);
                    lcd_puts(current->name);
                    done = 0;
                    found = 1;
                }

                current = current->next;
            }
        }

        /* In case we have no cards in list or we have no match */
        if (head == NULL || found == 0) {
            in_list = 0;
            PORTA &= ~_BV(PORTA1);
            since_blocked = seconds;
            lcd_clrscr();
            lcd_puts_P(PSTR(STUD_NAME));
            lcd_goto(0x40);
            lcd_puts_P(PSTR(ACCESS));
            done = 0;
        }

        /* Extending a local scope variable to global scope variable */
        in_list = found;
    }

    if (in_list == 1 && (seconds - since_allowed) < 3) {
        PORTA |= _BV(PORTA1);
    } else {
        PORTA &= ~_BV(PORTA1);
    }

    if (((seconds - since_blocked) >= 7) && ((seconds - since_allowed) >= 7) &&
            done == 0) {
        lcd_clrscr();
        lcd_puts_P(PSTR(STUD_NAME));
        done = 1;
    }
}


int main (void)
{
    init_rfid_reader();
    init_io();
    init_counter();
    print_info();
    init_microrl();
    /* Init LCD and display student name.*/
    lcd_init();
    lcd_clrscr();
    lcd_puts_P(PSTR(STUD_NAME));
    done = 1;

    while (1) {
        heartbeat();
        microrl_insert_char(prl, (uart0_getc() & UART_STATUS_MASK));
        check();
    }
}


ISR(TIMER1_COMPA_vect)
{
    seconds++;
}

