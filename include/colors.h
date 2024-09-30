#ifndef COLORS_H
#define COLORS_H

#define RESET 			"\033[0m"
#define BOLD 			"\033[1m"
#define DIM 			"\033[2m"
#define ITALIC 			"\033[3m"
#define UNDERLINE 		"\033[4m"
#define BLINK 			"\033[5m"
#define REVERSE 		"\033[7m"
#define HIDDEN 			"\033[8m"
#define BOLD_ITALIC 	"\033[3;1m"

#define BLACK			"\033[0;30m"
#define RED				"\033[0;31m"
#define GREEN			"\033[0;32m"
#define YELLOW			"\033[0;33m"
#define BLUE			"\033[0;34m"
#define MAGENTA			"\033[0;35m"
#define CYAN			"\033[0;36m"
#define WHITE			"\033[0;37m"
#define GRAY			"\033[90m"

#define DEBUG -1
#define INFO 0
#define WARNING 1
#define ERROR 2

#define MAX_DIRECTORY_PATH 1024

char* get_color_by_name(const char* color_name);
void println_colored(const char* message, char* message_color);
void print_colored(const char* message, char* message_color);
void println(const char* message);
void print_message(const char* message, int message_type);
void display_ps(int status);

#endif // COLORS_H
