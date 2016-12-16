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

#include <avr/pgmspace.h>
#ifndef _HMI_MSG_H_
#define _HMI_MSG_H_

#define PRO1 "Version: "
#define PRO2 " built on: "
#define LIBC "avr-libc version: "
#define AVR "avr-gcc version: "
#define STUD_NAME "Roomet Saadi"
#define MONTH "Enter Month name first letter >"
#define UPTIME "Uptime: %lu s"
#define UART_STATUS_MASK 0xFF
#define CMD_ERROR "\nCommand not implemented\n Use <help> to get help.\n"
#define CMD_ARG_ERROR "\nToo few or too many arguments for this command.\nUse <help>\n"
#define HELP1 "Implemented commands:\n"
#define HELP2 " : "
#define ACCESS "ACCESS DENIED!"

extern PGM_P const name_month[];

#endif

