/**
 * @title SheGang Linux Shell | User Input Module
 * @filename userinput.c
 * @brief Module with functions for read user input and split into tokens
 * @authors Alexeev Bronislav
 * @license MIT License
 **/
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "isocline.h"
#include "colorschemes.h"
#include "tasks_processing.h"
#include "shell_api.h"

extern ColorScheme* current_theme;

const char* builtin_utils[] = {"shegang_config", "sgls", "sglsblk", "help", "exit", 
									"term", "reload", "history", "gapf", "quit", "bg", "term", 
									"environment", "sghint", "echon", "sghint", NULL};

/**
 * @brief      Gets the system binaries.
 *
 * @return     The system binaries.
 */
char **get_sys_binaries(void) {
	DIR *dir;
	struct dirent *entry;
	char **progs = malloc(INIT_CAPACITY * sizeof(char *));
	size_t count = 0, capacity = INIT_CAPACITY;

	dir = opendir(BIN_DIR);
	if (dir == NULL) {
		fprintf(stderr, "Error opening dir %s: %s\n", BIN_DIR, strerror(errno));
		return NULL;
	}

	while ((entry = readdir(dir)) != NULL) {
		if (entry->d_name[0] == '.')
			continue;

		char path[128];
		snprintf(path, 128, BIN_DIR "/%s", entry->d_name);
		struct stat st;
		if (stat(path, &st) == 0 && (st.st_mode & S_IXUSR)) {
			if (count >= capacity) {
				capacity *= 2;
				progs = realloc(progs, capacity * sizeof(char *));
				if (progs == NULL) {
					fprintf(stderr, "Error reallocate memory: %s\n", strerror(errno));
					closedir(dir);
					return NULL;
				}
			}

			progs[count++] = strdup(entry->d_name);
		}
	}

	for (int i = 0; i < sizeof(builtin_utils) / sizeof(builtin_utils[0]); i++) {
		if (builtin_utils[i] == 0)
			break;

		progs[count++] = strdup(builtin_utils[i]);
	}

	progs = realloc(progs, (count + 1) * sizeof(char *));

	progs[count] = NULL;

	if (closedir(dir) != 0) {
		fprintf(stderr, "Error closing dir %s: %s\n", BIN_DIR, strerror(errno));
	}

	return progs;
}

/**
 * @brief      Word Auto Completer
 *
 * @param      cenv  The cenv
 * @param[in]  word  The word
 */
void word_completer(ic_completion_env_t* cenv, const char* word) {
	ic_add_completions(cenv, word, get_sys_binaries());

if (strcmp(word, "s") == 0) {
		ic_add_completion(cenv, "sgls");
		ic_add_completion(cenv, "sghint");
		ic_add_completion(cenv, "sglsblk");
		ic_add_completion(cenv, "shegang_config");
		ic_add_completion(cenv, "su");
		ic_add_completion(cenv, "sudo");
	} else if (strcmp(word, "sghint") == 0 || strcmp(word, "sghint ") == 0) {
		char* command_completion = (char*)malloc(1024);

		for (int i = 0; i < sizeof(builtin_utils) / sizeof(builtin_utils[0]); i++) {
			if (builtin_utils[i] == NULL) {
				break;
			}

			snprintf(command_completion, 1024, "sghint %s", builtin_utils[i]);

			ic_add_completion(cenv, command_completion);
		}
	} else if (strcmp(word, "shegang") == 0) {
		ic_add_completion(cenv, "shegang_config");
	} else if (strcmp(word, "t") == 0) {
		ic_add_completion(cenv, "top");
		ic_add_completion(cenv, "time");
		ic_add_completion(cenv, "touch");
		ic_add_completion(cenv, "tar");
		ic_add_completion(cenv, "touch");
		ic_add_completion(cenv, "tail");
		ic_add_completion(cenv, "term");
	} else if (strcmp(word, "e") == 0) {
		ic_add_completion(cenv, "environment");
	}
}

/**
 * @brief      Auto Completer
 *
 * @param      cenv   The cenv
 * @param[in]  input  The input
 */
void completer(ic_completion_env_t* cenv, const char* input) {
	ic_complete_filename(cenv, input, 0, ".;/usr/local;c:\\Program Files", NULL);

	ic_complete_word(cenv, input, &word_completer, NULL);
}

/**
 * @brief      Highlighter
 *
 * @param      henv   The henv
 * @param[in]  input  The input
 * @param      arg    The argument
 */
void highlighter(ic_highlight_env_t* henv, const char* input, void* arg) {
	long len = (long)strlen(input);

	for (long i = 0; i < len;) {
		const char* keywords[] = {"then", "if", "fi", "else", "do", "while", "function", "return", "not", "null",
								"false", "true", "and", "or", NULL};
		const char* commands[] = {"bash", "diff", "cat", "chown", "chmod", "chgrp", "cp", "dd", "du", "df", "ln",
								"ls", "mkdir", "mkfifo", "mv", "rm", "head", "tail", "md5sum", "basename",
								"chroot", "date", "echo", "env", "nice", "nohup", "sleep", "printf",
								"find", "xargs", "awk", "grep", "grub", "gzip", "unzip", "zip", "tar",
								"ssh", "telnet", "time", "make", "gcc", "clang", "gdb", "rmdir", "mkdir",
								"cd", NULL};
		const char* operators[] = {"==", ">=", "<=", "!=", ">", "<", "+", "-", "*", "/", "&&", "||", NULL};

		long tlen;

		if ((tlen = ic_match_any_token(input, i, &ic_char_is_idletter, keywords)) > 0) {
			ic_highlight(henv, i, tlen, get_color(current_theme, "keywords").hex);
			i += tlen;
		} else if ((tlen = ic_match_any_token(input, i, &ic_char_is_idletter, commands)) > 0) {
			ic_highlight(henv, i, tlen, get_color(current_theme, "commands").hex);
			i += tlen;
		} else if ((tlen = ic_match_any_token(input, i, &ic_char_is_idletter, builtin_utils)) > 0) {
			ic_highlight(henv, i, tlen, get_color(current_theme, "programs").hex);
			i += tlen;
		} else if ((tlen = ic_is_token(input, i, &ic_char_is_digit)) > 0) {
			ic_highlight(henv, i, tlen, get_color(current_theme, "digits").hex);
			i += tlen;
		} else if ((tlen = ic_is_token(input, i, &ic_char_is_separator)) > 0) {
			ic_highlight(henv, i, tlen, get_color(current_theme, "light_gray").hex);
			i += tlen;
		} else if (ic_starts_with(input + i, "#")) {
			tlen = 1;
			while (i + tlen < len && input[i + tlen] != '\n') {
				tlen++;
			}
			ic_highlight(henv, i, tlen, get_color(current_theme, "dark_gray").hex);
			i += tlen;
		} else if (ic_starts_with(input + i, "//")) {
			tlen = 2;
			while (i + tlen < len && input[i + tlen] != '\n') {
				tlen++;
			}
			ic_highlight(henv, i, tlen, get_color(current_theme, "dark_gray").hex);
			i += tlen;
		} else {
			if ((tlen = ic_match_any_token(input, i, &ic_char_is_nonwhite, operators)) > 0) {
				ic_highlight(henv, i, tlen, get_color(current_theme, "operators").hex);
				i += tlen;
			} else {
				ic_highlight(henv, i, 1, NULL);
				i++;
			}
		}
	}
}
