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

#ifndef _CLI_MICRORL_H_
#define _CLI_MICRORL_H_

void print (const char * str);
char cli_get_char(void);
void cli_print_help(void);
void cli_print_ver(void);
void cli_print_ascii_tbls(void);
void cli_handle_month(const char *const *argv);
void cli_print_cmd_error(void);
void cli_print_cmd_arg_error(void);
void read(void);
void add(const char *const *argv);
void remove(const char *const *argv);
void list(void);
int cli_execute(int argc, const char *const *argv);

#endif

