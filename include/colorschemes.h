#ifndef COLORSCHEMES_H
#define COLORSCHEMES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SCHEME_NAME_LENGTH 				64
#define MAX_COLOR_NAME_LENGTH				64
#define MAX_COLOR_HEX_LENGTH				8

typedef struct {
	char name[MAX_SCHEME_NAME_LENGTH];
	char hex[MAX_COLOR_HEX_LENGTH];
} Color;

typedef struct {
	char name[MAX_SCHEME_NAME_LENGTH];
	Color blue, red, orange, purple, yellow, green, cyan, white, black, light_gray, dark_gray;
	Color keywords, operators, digits, strings, comments, programs, commands;
} ColorScheme;

void set_color_scheme(ColorScheme* scheme);
void get_scheme_by_name(char* name);
Color get_color(ColorScheme* scheme, char* name);

#endif // COLORSCHEMES_H
