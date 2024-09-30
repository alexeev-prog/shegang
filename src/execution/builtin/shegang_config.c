/**
 * @title SheGang Linux Shell | Built-in util `shegang_config`
 * @filename shegang_config.c
 * @brief Module with shegang_config util
 * @authors Alexeev Bronislav
 * @license MIT License
 **/
#include <stdio.h>
#include <string.h>
#include "colors.h"
#include "colorschemes.h"
#include "userinput.h"

extern char* username_color;
extern char* pwd_color;
extern char* curr_time_color;

/**
 * @brief      Sets the value for variable.
 *
 * @param      var_name  The variable name
 * @param      value     The value
 *
 * @return     int       Status code
 */
int set_value_for_var(char* var_name, char* value) {
	char* color = get_color_by_name(value);

	if (strcmp(var_name, "USERNAME_COLOR") == 0) {
		username_color = color;
	} else if (strcmp(var_name, "PWD_COLOR") == 0) {
		pwd_color = color;
	} else if (strcmp(var_name, "TIME_COLOR") == 0) {
		curr_time_color = color;
	} else {
		print_message("Expected argument for \"shegang_config\" command: variable name", WARNING);
	}

	return 1;
}

/**
 * @brief      Function for `shegang_config` command
 *
 * @param      args  The arguments
 *
 * @return     int   Status code
 */
int shegang_config(char** args) {
	if (args[1] == NULL) {
		print_message("Expected argument for \"shegang_config\" command. Launch with help for view help page", WARNING);
		return 1;
	} else if (strcmp(args[1], "help") == 0 || strcmp(args[1], "-h") == 0 || strcmp(args[1], "--help") == 0) {
		println("Built-in function `shegang_config`\n");
		printf(
			"set <VAR> <VALUE>           - set value for var (ex. set USERNAME_COLOR RED)\n"
			"\nExisting variables: USERNAME_COLOR; PWD_COLOR; TIME_COLOR\n"
			"Existing colors: RED, GREEN, BLUE, YELLOW, MAGENTA, GRAY, BLACK, WHITE, CYAN\n"
			"Special variables: COLORSCHEME (values: onedark, gruvbox)"
		);
	} else if (strcmp(args[1], "set") == 0) {
		if (args[3] == NULL || args[2] == NULL) {
			print_message("Expected argument for \"shegang_config\" command: color", WARNING);
			return 1;
		}

		if (strcmp(args[2], "COLORSCHEME") == 0) {
			get_scheme_by_name(args[3]);
			setup_isocline();
			return 1;
		}

		char* color = get_color_by_name(args[3]);

		return set_value_for_var(args[2], color);
	}

	return 1;
}
