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
#include <avr/pgmspace.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>

#include "../lib/hd44780_111/hd44780.h"
#include "hmi_msg.h"
#include "../lib/andygock_avr-uart/uart.h"
#include "print_helper.h"
#include "cli_microrl.h"
#include "../lib/matejx_avr_lib/mfrc522.h"
#include "rfid.h"

#define UART_STATUS_MASK 0xFF


void print (const char * str)
{
    fprintf (stdout, "%s", str);
}


typedef struct cli_cmd {
    PGM_P cmd;
    PGM_P help;
    void (*func_p)();
    const uint8_t func_argc;
} cli_cmd_t;


const char help_cmd[] PROGMEM = "help";
const char help_help[] PROGMEM = "Get help";
const char ver_cmd[] PROGMEM = "version";
const char ver_help[] PROGMEM = "Print FW version";
const char ascii_cmd[] PROGMEM = "ascii";
const char ascii_help[] PROGMEM = "print ASCII tables";
const char month_cmd[] PROGMEM = "month";
const char month_help[] PROGMEM =
    "Find matching month from lookup list. Usage: month <string>";
const char read_cmd[] PROGMEM = "read";
const char read_help[] PROGMEM = "Read UID from RFID card";
const char add_cmd[] PROGMEM = "add";
const char add_help[] PROGMEM =
    "Add card to memory by name (card needs to be in contact)";
const char remove_cmd[] PROGMEM = "remove";
const char remove_help[] PROGMEM = "Remove card from memory by name";
const char list_cmd[] PROGMEM = "list";
const char list_help[] PROGMEM = "List all valid cards";


const cli_cmd_t cli_cmds[] = {
    {help_cmd, help_help, cli_print_help, 0},
    {ver_cmd, ver_help, cli_print_ver, 0},
    {ascii_cmd, ascii_help, cli_print_ascii_tbls, 0},
    {month_cmd, month_help, cli_handle_month, 1},
    {read_cmd, read_help, read, 0},
    {add_cmd, add_help, add, 1},
    {remove_cmd, remove_help, remove, 1},
    {list_cmd, list_help, list, 0}
};


void cli_print_help(void)
{
    printf_P(PSTR("\n"));
    printf_P(PSTR(HELP1));

    for (uint8_t i = 0; i < (sizeof(cli_cmds) / sizeof(cli_cmds[1])); i++) {
        printf_P(cli_cmds[i].cmd);
        printf_P(PSTR(HELP2));
        printf_P(cli_cmds[i].help);
        printf_P(PSTR("\n"));
    }
}


void cli_print_ver(void)
{
    printf_P(PSTR("\n"));
    printf_P(PSTR(PRO1 GIT_DESCR PRO2 __DATE__ " " __TIME__ "\n"));
    printf_P(PSTR(LIBC __AVR_LIBC_VERSION_STRING__ " " AVR __VERSION__ "\n"));
}


void cli_print_ascii_tbls(void)
{
    printf_P(PSTR("\n"));
    /* Print ASCII table */
    print_ascii_tbl(stdout);
    /* Create and initialize an array of 128 zeros! */
    unsigned char array[128];

    for (int i = 0; i < 128; i++) {
        array[i] = i;
    }

    /*  Print just created array out for humans in stdout! */
    print_for_human(stdout, array, 128);
}


/* Month name and acquired char comparison and printout in stdout! */
void cli_handle_month(const char *const *argv)
{
    char outbuf[20] = "";
    lcd_clrscr();
    lcd_puts_P(PSTR(STUD_NAME));
    lcd_goto(0x40);
    int len;

    for (len = 0; argv[1][len] != '\0'; len++);

    for (unsigned int i = 0; i < 6; i++) {
        strcpy_P(outbuf, (PGM_P)pgm_read_word(&(name_month[i])));

        if (strncmp(argv[1], outbuf, len) == 0) {
            lcd_puts(outbuf);
            lcd_puts(" ");
            printf("\n");
            printf(outbuf);
        }
    }

    printf("\n");
}


void cli_print_cmd_error(void)
{
    printf_P(PSTR(CMD_ERROR));
}


void cli_print_cmd_arg_error(void)
{
    printf_P(PSTR(CMD_ARG_ERROR));
}


void read(void)
{
    Uid uid;
    Uid *uid_ptr = &uid;
    printf_P(PSTR("\n"));

    if (PICC_IsNewCardPresent()) {
        printf("Card selected!\n");
        PICC_ReadCardSerial(uid_ptr);
        printf("UID size: 0x%02X\n", uid.size);
        printf("UID sak: 0x%02X\n", uid.sak);
        printf("Card UID: ");

        for (byte i = 0; i < uid.size; i++) {
            printf("%02X", uid.uidByte[i]);
        }

        printf_P(PSTR("\n"));
    } else {
        printf_P((PSTR("No card detected!\n")));
    }
}


void add(const char *const *argv)
{
    /* To deal with situation, when we try to add card already in list. */
    int is_found = 0;

    if (PICC_IsNewCardPresent()) {
        if (head != NULL) {
            card_t *current;
            current = head;
            int number = 1;

            while (current != NULL && is_found == 0) {
                uint8_t *uid = current->uid;
                Uid card_uid;
                Uid *uid_ptra = &card_uid;
                PICC_ReadCardSerial(uid_ptra);

                /* In case we have a match in list with same UID or same name. Then we declare card as "is_found" and we skip add operation. */
                if (!memcmp(current->name, argv[1], (strlen(current->name) + 1)) ||
                        !memcmp(card_uid.uidByte, current->uid, card_uid.size)) {
                    is_found = 1;
                    printf("%d. ", number);

                    for (byte i = 0; i < current->size; i++) {
                        printf("%02X", uid[i]);
                    }

                    printf(" %s", current->name);
                }

                current = current->next;
                number++;
            }
        }

        /* If card is declared as "is_found" then the add operation is skipped */
        if (is_found == 0) {
            card_t *new_card;
            Uid uid;
            Uid *uid_ptr = &uid;
            new_card = malloc(sizeof(card_t));
            PICC_ReadCardSerial(uid_ptr);
            memcpy(&(new_card->size), &(uid.size), 1);
            memcpy(new_card->uid, uid.uidByte, uid.size);
            new_card->name = malloc(strlen(argv[1]) + 1);

            if (new_card == NULL || new_card->uid == NULL || new_card->name == NULL) {
                printf_P(PSTR("Not enough memory. Please remove any of the cards added."));
            }

            strcpy(new_card->name, argv[1]);
            new_card->next = NULL;

            if (head != NULL) {
                card_t *current;
                current = head;

                while (current->next != NULL) {
                    current = current->next;
                }

                current->next = new_card;
            } else {
                head = new_card;
            }
        }
    } else if (is_found == 0) {
        printf_P(PSTR("No card detected!"));
    }

    printf_P(PSTR("\n"));
}


void remove(const char *const *argv)
{
    card_t *current;
    card_t *previous;
    current = head;
    previous = head;
    int in_list = 0;

    while (current != NULL) {
        if (current == head && current->next == NULL &&
                !memcmp(current->name, argv[1], (strlen(current->name) + 1))) {
            head = NULL;
            free(current->name);
            free(current);
            current = NULL;
            previous = NULL;
            in_list = 1;
        } else if (current == head && current->next != NULL &&
                   !memcmp(current->name, argv[1], (strlen(current->name) + 1))) {
            head = current-> next;
            free(current->name);
            free(current);
            current = head;
            previous = head;
            in_list = 1;
        } else if (current->next != NULL &&
                   !memcmp(current->name, argv[1], (strlen(current->name) + 1))) {
            previous->next = current->next;
            free(current->name);
            free(current);
            current = previous->next;
            in_list = 1;
        } else if (current->next == NULL  &&
                   !memcmp(current->name, argv[1], (strlen(current->name) + 1))) {
            previous->next = NULL;
            free(current->name);
            free(current);
            current = NULL;
            in_list = 1;
        } else if (current->next != NULL) {
            previous = current;
            current = current->next;
        } else {
            previous = current;
            current = NULL;
        }
    }

    if (in_list == 0) {
        printf("No card with that name!");
    }

    printf_P(PSTR("\n"));
}


void list(void)
{
    if (head == NULL) {
        printf_P(PSTR("No cards added to the list!"));
    } else {
        int number = 1;
        card_t *current;
        current = head;

        while (current->next != NULL) {
            printf("%d. ", number);
            uint8_t *uid = current->uid;

            for (byte i = 0; i < current->size; i++) {
                printf("%02X", uid[i]);
            }

            printf(" %s", current->name);
            current = current->next;
            number++;
            printf_P(PSTR("\n"));
        }

        printf("%d. ", number);
        uint8_t *uid = current->uid;

        for (byte i = 0; i < current->size; i++) {
            printf("%02X", uid[i]);
        }

        printf(" %s", current->name);
    }

    printf_P(PSTR("\n"));
}


int cli_execute(int argc, const char *const *argv)
{
    /** Test every possible command defined, if it matches command entered **/
    for (uint8_t i = 0; i < (sizeof(cli_cmds) / sizeof(cli_cmds[0])); i++) {
        /** If match is found **/
        if (!strcmp_P(argv[0], cli_cmds[i].cmd)) {
            /** In case the argument count doesn't match **/
            if ((argc - 1) != cli_cmds[i].func_argc) {
                cli_print_cmd_arg_error();
                return 0;
            }

            cli_cmds[i].func_p(argv);
            return 0;
        }
    }

    cli_print_cmd_error();
    return 0;
}

