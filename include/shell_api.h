#ifndef SHELL_API_H
#define SHELL_API_H

#define CONFIG_FILE					"/.shegangrc"
#define CONFIG_ALIASES 				"/.sgaliases"
#define DEFAULT_USERNAME_COLOR  	"\033[0;33m"
#define DEFAULT_PWD_COLOR			"\033[0;32m"
#define DEFAULT_CURR_TIME_COLOR 	"\033[0;35m"

#define MAX_LINE_LENGTH				256
#define MAX_ALIASES					128

#define DEFAULT_BUFFER_SIZE 		256
#define TOKENS_DELIMETERS 			" \t"

#define MAX_FILE_NAME_LENGTH		1024
#define MAX_GRID_COLUMNS			3

#define INIT_CAPACITY				1024
#define MAX_PROGRAMS 				4096
#define BIN_DIR 					"/bin"

#define RESET 						"\x1b[0m"
#define BOLD 						"\x1b[1m"
#define DIM 						"\x1b[2m"
#define ITALIC 						"\x1b[3m"
#define UNDERLINE 					"\x1b[4m"
#define BLINK 						"\x1b[5m"
#define REVERSE 					"\x1b[7m"
#define HIDDEN 						"\x1b[8m"

#define BLACK						"\x1b[30m"
#define RED							"\x1b[31m"
#define GREEN						"\x1b[32m"
#define YELLOW						"\x1b[33m"
#define BLUE						"\x1b[34m"
#define MAGENTA						"\x1b[35m"
#define CYAN						"\x1b[36m"
#define WHITE						"\x1b[37m"
#define GRAY						"\x1b[90m"

#define DEBUG 						-1
#define INFO 						0
#define WARNING 					1
#define ERROR 						2

#define MAX_DIRECTORY_PATH 			2048

#define MAX_SCHEME_NAME_LENGTH 		64
#define MAX_COLOR_NAME_LENGTH		64
#define MAX_COLOR_HEX_LENGTH		8

#endif // SHELL_API_H
