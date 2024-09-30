/**
 * @title SheGang Linux Shell
 * @filename shegang.c
 * @brief Simple command interpreter (shell) for linux written in C
 * @authors Alexeev Bronislav
 * @license MIT License
 **/
#include <stdio.h>
#include <unistd.h>
#include <locale.h>
#include "userinput.h"
#include "executor.h"
#include "tasks_processing.h"
#include "plugins_manager.h"
#include "config.h"
#include "isocline.h"

extern tasks tasks_structure;
extern char* username_color;
extern char* pwd_color;

/**
 * @brief      Prints a welcome message.
 */
void print_welcome_message(void) {
	printf(
		"%s     ____ %s   %s%sSHE#GANG - powerful command interpreter (shell) for linux written in C%s\n"
		"%s  __/ / /_%s   %s%sBlazing fast, cool, simple shell in C%s\n"
		"%s /_  . __/%s   %s%sdeveloped by alxvdev%s\n"
		"%s/_    __/ %s   %s%shttps://github.com/alxvdev/shegang%s\n"
		"%s /_/_/    %s   %sMIT License%s\n\n", GREEN,
		RESET, GREEN, BOLD, RESET,
		GREEN, RESET, GREEN, ITALIC, RESET,
		GREEN, RESET, CYAN, DIM, RESET,
		GREEN, RESET, CYAN, UNDERLINE, RESET,
		GREEN, RESET, DIM, RESET
	);
}

/**
 * @brief      main function
 *
 * @param[in]  argc  The count of arguments
 * @param      argv  The arguments array
 *
 * @return     status of program
 */
int main(int argc, char** argv) {
	setlocale(LC_ALL, "C.UTF-8");

	char* line;
	char** args;
	char** args_by_delims;
	int status = 1;
	int returned_value = 0;
	int have_plugin = 0;
	int num_tokens;
	
	const char* delims[] = {"&&", "||", ";"};
	const int delims_len = sizeof(delims) / sizeof(delims[0]);

	load_main_config();
	load_config_aliases();
	setup_isocline();

	print_welcome_message();

	plugin_t myplugin;

	if (argv[1] != NULL) {
		have_plugin = 1;
	}

	if (have_plugin) {
		if (load_plugin(argv[1], &myplugin) == -1) {
			fprintf(stderr, "Plugin loading error\n");
			return -1;
		}

		myplugin.functions.init();
		print_plugin_info(&myplugin);
	}

	ic_async_stop();

	signal(SIGINT, kill_foreground_task);
	signal(SIGCHLD, mark_ended_task);

	do {
		display_ps(status);

		line = read_user_input();

		if (line == NULL) {
			continue;
		}

		if (have_plugin) {
			myplugin.functions.mainloop(line);
		}

		args_by_delims = split_by_delims(line, "&&||;", &num_tokens);

		for (int i = 0; i < num_tokens; i++) {
			args = split_into_tokens(args_by_delims[i]);
			status = execute(args);
		}

		free(line);
		free(args);
	} while(status >= 1 || status < 0);

	return 0;
}
