/**
 * @title SheGang Linux Shell | Colorschemes
 * @filename colorshemes.c
 * @brief Module with colorschemes
 * @authors Alexeev Bronislav
 * @license MIT License
 **/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell_api.h"

typedef struct {
	char name[MAX_SCHEME_NAME_LENGTH];
	char hex[MAX_COLOR_HEX_LENGTH];
} Color;

typedef struct {
	char name[MAX_SCHEME_NAME_LENGTH];
	Color blue, red, orange, purple, yellow, green, cyan, white, black, light_gray, dark_gray;
	Color keywords, operators, digits, strings, comments, programs, commands;
} ColorScheme;

ColorScheme gruvbox = {
	.name = "gruvbox",
	.blue = {"blue", "#83a598"},
	.red = {"red", "#fb4934"},
	.orange = {"orange", "#fe8019"},
	.purple = {"purple", "#d3869b"},
	.yellow = {"yellow", "#fabd2f"},
	.green = {"green", "#b8bb26"},
	.cyan = {"cyan", "#8ec07c"},
	.white = {"white", "#edbbb2"},
	.black = {"black", "#282828"},
	.light_gray = {"light_gray", "#b9aa95"},
	.dark_gray = {"dark_gray", "#504945"},
	.keywords = {"keywords", "#83a598"},
	.operators = {"operators", "#fb4934"},
	.digits = {"digits", "#b8bb26"},
	.strings = {"strings", "#b8bb26"},
	.comments = {"comments", "#a89984"},
	.programs = {"programs", "#d3869b"},
	.commands = {"commands", "#fe8019"}
};

ColorScheme onedark = {
	.name = "gruvbox",
	.blue = {"blue", "#61afef"},
	.red = {"red", "#e06c75"},
	.orange = {"orange", "#d19a66"},
	.purple = {"purple", "#c678dd"},
	.yellow = {"yellow", "#e5c07b"},
	.green = {"green", "#98c379"},
	.cyan = {"cyan", "#56b6c2"},
	.white = {"white", "#abb2bf"},
	.black = {"black", "#282c34"},
	.light_gray = {"light_gray", "#9aa1ae"},
	.dark_gray = {"dark_gray", "#3e4452"},
	.keywords = {"keywords", "#61afef"},
	.operators = {"operators", "#e06c75"},
	.digits = {"digits", "#98c379"},
	.strings = {"strings", "#98c379"},
	.comments = {"comments", "#9aa1ae"},
	.programs = {"programs", "#c678dd"},
	.commands = {"commands", "#d19a66"}
};

ColorScheme* current_theme = &gruvbox;

/**
 * @brief      Sets the color scheme.
 *
 * @param      scheme  The scheme
 */
void set_color_scheme(ColorScheme* scheme) {
	current_theme = scheme;
}

/**
 * @brief      Gets the scheme by name.
 *
 * @param      name  The name
 */
void get_scheme_by_name(char* name) {
	if (strcmp(name, "gruvbox") == 0) {
		set_color_scheme(&gruvbox);
		return;
	} else if (strcmp(name, "onedark") == 0) {
		set_color_scheme(&onedark);
		return;
	}
	
	set_color_scheme(&gruvbox);
}

/**
 * @brief      Gets the color.
 *
 * @param      scheme  The scheme
 * @param      name    The name
 *
 * @return     The color.
 */
Color get_color(ColorScheme* scheme, char* name) {
	struct {char *text; Color color; } colors[] = {
		{.text="blue"		, .color = scheme->blue},
		{.text="red"		, .color = scheme->red},
		{.text="orange"		, .color = scheme->orange},
		{.text="yellow"		, .color = scheme->yellow},
		{.text="green"		, .color = scheme->green},
		{.text="cyan"		, .color = scheme->cyan},
		{.text="white"		, .color = scheme->white},
		{.text="black"		, .color = scheme->black},
		{.text="light_gray"	, .color = scheme->light_gray},
		{.text="dark_gray"	, .color = scheme->dark_gray},
		{.text="keywords"	, .color = scheme->keywords},
		{.text="operators"	, .color = scheme->operators},
		{.text="purple"		, .color = scheme->purple},
		{.text="digits"		, .color = scheme->digits},
		{.text="strings"	, .color = scheme->strings},
		{.text="comments"	, .color = scheme->comments},
		{.text="programs"	, .color = scheme->programs},
		{.text="commands"	, .color = scheme->commands},
	};

	const int len = sizeof(colors) / sizeof(colors[0]);

	for (int i = 0; i < len; ++i) {
		if (strcmp(name, colors[i].text) == 0) {
			return colors[i].color;
		}
	}

	return (Color){.name = "invalid", .hex="#FFFFFF"};
}
