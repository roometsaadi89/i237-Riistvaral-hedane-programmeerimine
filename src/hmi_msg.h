#ifndef _HMI_MSG_H_
#define _HMI_MSG_H_
#include <avr/pgmspace.h>
#define UPTIME "Uptime: %lu s"
#define STUD_NAME "Roomet Saadi"
#define VER_FW "Version: %S built on: %S %S"
#define VER_LIBC "avr-libc version: %S avr-gcc version : %S"

#define HELP_CMD "help"
#define HELP_HELP "Get help"
#define VER_CMD "version"
#define VER_HELP "Print FW version"
#define ASCII_CMD "ascii"
#define ASCII_HELP "print ASCII tables"
#define MONTH_CMD "month"
#define MONTH_HELP "Find matching month from lookup list. Usage: month <string>"
#define CLI_HELP_MSG "Implemented commands:"
#define CLI_NO_CMD "Command not implemented.\n Use <help> to get help."
#define CLI_ARGS_MSG "To few or to many arguments for this command\nUse <help>"

extern PGM_P const month_lst[];

extern const char help_cmd[];
extern const char help_help[];
extern const char ver_cmd[];
extern const char ver_help[];
extern const char ascii_cmd[];
extern const char ascii_help[];
extern const char month_cmd[];
extern const char month_help[];

#endif /* _HMI_MSG_H_ */
