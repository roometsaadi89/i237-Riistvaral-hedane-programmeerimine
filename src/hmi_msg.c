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
#include "hmi_msg.h"

const char s1[] PROGMEM = "January";
const char s2[] PROGMEM = "February";
const char s3[] PROGMEM = "March";
const char s4[] PROGMEM = "April";
const char s5[] PROGMEM = "May";
const char s6[] PROGMEM = "June";

PGM_P const name_month[] PROGMEM = {s1, s2, s3, s4, s5, s6};

