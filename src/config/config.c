/**
 * @title SheGang Linux Shell | Config reader
 * @filename colors.c
 * @brief Module with config reader and settings for shegang
 * @authors Alexeev Bronislav
 * @license MIT License
 **/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "colors.h"
#include "colorschemes.h"
#include "shell_api.h"

char* username_color;
char* pwd_color;
char* curr_time_color;

typedef struct {
	char name[64];
	char command[256];
} Alias;

Alias aliases[MAX_ALIASES];
int num_aliases = 0;

/**
 * load_config
 * 
 * @brief      Loads configuration aliases.
 */
void load_config_aliases(void) {
	char *home_dir = getenv("HOME");
	
	char config_path[strlen(home_dir) + strlen(CONFIG_ALIASES) + 1];
	sprintf(config_path, "%s%s", home_dir, CONFIG_ALIASES);
	FILE* config_file = fopen(config_path, "r");

	if (!config_file) {
		return;
	}

	FILE *fp;
	char alias_file[strlen(home_dir) + 11];
	sprintf(alias_file, "%s%s", home_dir, CONFIG_ALIASES);

	fp = fopen(alias_file, "r");

	if (fp == NULL) {
		print_message("Could not open config", ERROR);
		return;
	}

	char line[MAX_LINE_LENGTH];

	while (fgets(line, MAX_LINE_LENGTH, fp) != NULL && num_aliases < MAX_ALIASES) {
		char *alias_name = strtok(line, "=");
		char *alias_cmd = strtok(NULL, "\n");

		if (alias_name != NULL && alias_cmd != NULL) {
			strncpy(aliases[num_aliases].name, alias_name, sizeof(aliases[num_aliases].name) - 1);
			strncpy(aliases[num_aliases].command, alias_cmd, sizeof(aliases[num_aliases].command) - 1);
			num_aliases++;
		}
	}

	fclose(fp);
}

/**
 * load_config

 @brief      Function for load shell prompt colors from config
*/
void load_main_config(void) {
	char* home_dir = getenv("HOME");
	char line[256];

	if (!home_dir) {
		username_color = DEFAULT_USERNAME_COLOR;
		pwd_color = DEFAULT_PWD_COLOR;
		curr_time_color = DEFAULT_CURR_TIME_COLOR;
		return;
	}

	char config_path[strlen(home_dir) + strlen(CONFIG_FILE) + 1];
	sprintf(config_path, "%s%s", home_dir, CONFIG_FILE);

	FILE* config_file = fopen(config_path, "r");

	if (!config_file) {
		username_color = DEFAULT_USERNAME_COLOR;
		pwd_color = DEFAULT_PWD_COLOR;
		return;
	}

	while (fgets(line, sizeof(line), config_file)) {
		char* key = strtok(line, "=");
		char* value = strtok(NULL, "\n");

		if (key && value) {
			if (strcmp(key, "USERNAME_COLOR") == 0) {
				username_color = get_color_by_name(value);
			} else if (strcmp(key, "PWD_COLOR") == 0) {
				pwd_color = get_color_by_name(value);
			} else if (strcmp(key, "TIME_COLOR") == 0) {
				curr_time_color = get_color_by_name(value);
			} else if (strcmp(key, "COLORSCHEME") == 0) {
				get_scheme_by_name(value);
			}
		}
	}

	fclose(config_file);

	if (!username_color) {
		username_color = DEFAULT_USERNAME_COLOR;
	}
	if (!pwd_color) {
		pwd_color = DEFAULT_PWD_COLOR;
	}
	if (!curr_time_color) {
		curr_time_color = DEFAULT_CURR_TIME_COLOR;
	}
}
