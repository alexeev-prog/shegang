/**
 * @title SheGang Linux Shell | User Input Module
 * @filename userinput.c
 * @brief Module with functions for read user input and split into tokens
 * @authors Alexeev Bronislav
 * @license MIT License
 **/
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "isocline.h"
#include "colors.h"
#include "autocomplete.h"
#include "shell_api.h"

/**
 * @brief      get line from filename by line number
 *
 * @param      filename  The filename
 * @param[in]  line_n    The line n
 *
 * @return     line
 */
char* r_line(char* filename, int line_n) {
	FILE* file = fopen(filename, "r");
	char line[256];

	char *data = malloc(sizeof(char) * 256);

	int i = 0;

	while (fgets(line, sizeof(line), file)) {
		i++;

		if (i == line_n)
			strcpy(data, line);
	}

	fclose(file);

	return data;
}

/**
 * @brief      Gets the history file path.
 *
 * @return     The history file path.
 */
char* get_history_file_path(void) {
	char* home_dir = getenv("HOME");

	if (home_dir == NULL) {
		return NULL;
	}

	char* history_file_path = (char*)malloc(1024);

	if (history_file_path == NULL) {
		return NULL;
	}

	snprintf(history_file_path, 1024, "%s/.sghistory", home_dir);

	return history_file_path;
}

/**
 * @brief      Setup isocline settings
 */
void setup_isocline(void) {
	ic_style_def("kbd", "white underline");
	ic_style_def("ic-prompt", "gray");

	printf("%s\n", get_history_file_path());
	ic_set_history(get_history_file_path(), 1024);

	ic_set_default_completer(&completer, NULL);
	ic_set_default_highlighter(highlighter, NULL);

	ic_enable_auto_tab(true);
	ic_set_hint_delay(100);
	ic_enable_brace_matching(true);
	ic_enable_brace_insertion(true);
	ic_enable_inline_help(true);
}

/**
 * @brief      Function for read user input with isocline lib
 *
 * @return     char* Input buffer
 */
char* read_user_input(void) {
	printf("\n");

	char* input_buffer = ic_readline("╰─");

	// If input buffer is not plain string
	if (input_buffer == NULL) {
		print_message("Failed to read user input", WARNING);
		return NULL;
	}

	// if input buffer is not empty
	if (*input_buffer != '\0') {
		ic_history_add(input_buffer);
	}

	return input_buffer;
}

/**
 * @brief      Splits by delims.
 *
 * @param      line   The line
 * @param      delims  The delimiters
 *
 * @return     splitted line
 */
char** split_by_delims(char* line, const char* delims, int *num_tokens) {
	char **tokens = NULL;
	char *token;
	int i = 0;

	char *line_copy = malloc(strlen(line) + 1);
	strcpy(line_copy, line);

	token = strtok(line_copy, delims);

	while (token != NULL) {
		tokens = realloc(tokens, (i + 1) * sizeof(char *));
		tokens[i] = malloc(strlen(token) + 1);
		strcpy(tokens[i], token);
		i++;
		token = strtok(NULL, delims);
	}

	*num_tokens = i;
	free(line_copy);
	return tokens;
}

/**
 * @brief      function for split line into tokens
 *
 * @param      line  The line
 * @param      char*  line Line for splitting
 *
 * @return     char**
 */
char** split_into_tokens(char* line) {
	size_t position = 0;
	size_t buffer_size = DEFAULT_BUFFER_SIZE;

	char* token;
	char** tokens = (char**)malloc(sizeof(char*) * buffer_size);

	// If line is empty or failed to allocate memory
	if (tokens == NULL) {
		print_message("Couldn't allocate buffer for splitting", ERROR);
		return NULL;
	}

	// tokenize line
	token = strtok(line, TOKENS_DELIMETERS);

	while (token != NULL) {
		tokens[position++] = token;

		if (position >= buffer_size) {
			buffer_size *= 2;

			tokens = (char**)realloc(tokens, buffer_size * sizeof(char*));

			if (tokens == NULL) {
				print_message("Couldn't reallocate buffer for tokens", ERROR);
				return NULL;
			}
		}

		token = strtok(NULL, TOKENS_DELIMETERS);
	}

	tokens[position] = NULL;

	return tokens;
}
