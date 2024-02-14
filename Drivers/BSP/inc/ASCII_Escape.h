/*
 * ASCII_Escape.h
 *
 *  Created on: Jan 9, 2024
 *      Author: mm940
 */

#ifndef __ASCII_ESCAPE_H__
#define __ASCII_ESCAPE_H__


// Definitions
#define AES                   "\033"
#define AES_RESET             "\033[0m"
#define AES_RED_FG            "\033[31m"
#define AES_GREEN_FG          "\033[32m"
#define AES_YELLOW_FG         "\033[33m"
#define AES_BLUE_FG           "\033[34m"
#define AES_MAGENTA_FG        "\033[35m"
#define AES_CYAN_FG           "\033[36m"
#define AES_WHITE_FG          "\033[37m"
#define AES_BRIGHT_RED_FG     "\033[31;1m"
#define AES_BRIGHT_GREEN_FG   "\033[32;1m"
#define AES_BRIGHT_YELLOW_FG  "\033[33;1m"
#define AES_BRIGHT_BLUE_FG    "\033[34;1m"
#define AES_BRIGHT_MAGENTA_FG "\033[35;1m"
#define AES_BRIGHT_CYAN_FG    "\033[36;1m"
#define AES_BRIGHT_WHITE_FG   "\033[37;1m"

#define AES_DEF_FG            "\033[39m"      // Default Foreground color
#define AES_RED_BG            "\033[41m"
#define AES_GREEN_BG          "\033[42m"
#define AES_YELLOW_BG         "\033[43m"
#define AES_BLUE_BG           "\033[44m"
#define AES_MAGENTA_BG        "\033[45m"
#define AES_CYAN_BG           "\033[46m"
#define AES_WHITE_BG          "\033[47m"
#define AES_DEF_BG            "\033[49m"      // Default Background color


#define AES_BLINK       "\033[5m"
#define AES_CTRL_Z      "\x1A"
#endif  // __ASCII_ESCAPE_H__
