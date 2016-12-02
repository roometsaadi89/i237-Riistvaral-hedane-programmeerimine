#include <avr/pgmspace.h>
#include "hmi_msg.h"

static const char january[] PROGMEM = "January";
static const char february[] PROGMEM = "February";
static const char march[] PROGMEM = "March";
static const char april[] PROGMEM = "April";
static const char may[] PROGMEM = "May";
static const char june[] PROGMEM = "June";

PGM_P const month_lst[] PROGMEM = {january, february, march, april, may, june};

const char help_cmd[] PROGMEM = HELP_CMD;
const char help_help[] PROGMEM = HELP_HELP;
const char ver_cmd[] PROGMEM = VER_CMD;
const char ver_help[] PROGMEM = VER_HELP;
const char ascii_cmd[] PROGMEM = ASCII_CMD;
const char ascii_help[] PROGMEM = ASCII_HELP;
const char month_cmd[] PROGMEM = MONTH_CMD;
const char month_help[] PROGMEM = MONTH_HELP;

