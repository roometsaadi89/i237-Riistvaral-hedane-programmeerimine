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

#ifndef _RFID_H_
#define _RFID_H_
#include <inttypes.h>

#ifndef NULL
#define NULL ((void *)0)
#endif

typedef struct card {
    uint8_t size;
    uint8_t uid[10];
    char *name;
    struct card *next;
} card_t;

extern card_t *head;

void check(void);

#endif

