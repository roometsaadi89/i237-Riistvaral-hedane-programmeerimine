#ifndef _HMI_MSG_H_
#define _HMI_MSG_H_
#include <avr/pgmspace.h>
#define UPTIME "Uptime: %lu s"

const char STUD_NAME[] PROGMEM = "Roomet Saadi";
const char VER_FW[] PROGMEM = "Version: %S built on: %S %S\n";
const char VER_LIBC[] PROGMEM = "avr-libc version: %S\n";
const char MONTH_MSG[] PROGMEM = "Enter Month name first letter >";

const char january[] PROGMEM = "January";
const char february[] PROGMEM = "February";
const char march[] PROGMEM = "March";
const char april[] PROGMEM = "April";
const char may[] PROGMEM = "May";
const char june[] PROGMEM = "June";

PGM_P const months[] PROGMEM = {january,february,march,april,may,june};

#endif /* _HMI_MSG_H_ */

