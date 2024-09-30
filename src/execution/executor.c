/**
 * @title SheGang Linux Shell | Execute and basic built-in functions
 * @filename executor.c
 * @brief Module with command executor and basic built-in functions
 * @authors Alexeev Bronislav
 * @license MIT License
 **/
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <linenoise.h>
#include <regex.h>
#include "colors.h"
#include "tasks_processing.h"
#include "userinput.h"
#include "config.h"
#include "builtin.h"
#include "executor.h"
#include "environment.h"
#include "isocline.h"

extern tasks tasks_structure;
extern Alias aliases[MAX_ALIASES];
extern int num_aliases;

extern int global_status_code;

/**
 * @brief      Function for print info about shegang
 *
 * @return     int Status code
 */
int help(char** args) {
	println("she#gang Linux Shell in C  @  by alxvdev\n");

	if (args[1] == NULL) {
		printf(
			"Built-in shell special functions:\n\n"
			
			" cd     <path>           - Change the directory\n"
			" term   <bg_task_idx>    - Kill background task by id\n"
			" help                    - Prints info about she#gang\n"
			" bg                      - Prints list with background tasks\n"
			" quit/exit               - Terminate shell with all active tasks\n"
			" reload                  - Reload the shell and config\n"
			" shegang_config          - Configurate the some config values in current session without saving (help exists)\n"
			" history                 - Print the shell commands history\n"
			" sgls                    - `ls` command analogue, with colors and nerd icons (help exists)\n"
			" sglsblk                 - `lsblk` command analogue\n"
			" environment             - Set and get env vars\n"
			" echon                   - `echo -n` command analogue\n"
			" sghint                  - small util for view built-in command hints\n"
			" gapf                    - `cat` command simple analogue\n\n"

			" Additional arguments: shegang_config, sgls; environment\n"
		);
	} else if (strcmp(args[1], "shegang_config") == 0) {
		println("Built-in function `shegang_config`\n");
		printf(
			"set <VAR> <VALUE>           - set value for var (ex. set USERNAME_COLOR RED)\n"
			"\nExisting variables: USERNAME_COLOR; PWD_COLOR; TIME_COLOR;\n"
			"Existing colors: RED, GREEN, BLUE, YELLOW, MAGENTA, GRAY, BLACK, WHITE, CYAN\n"
		);
	} else if (strcmp(args[1], "sgls") == 0) {
		println("Built-in function `sgls`\n");
		printf(
				"SGLS - A `ls` command analogue from shegang\n\n"
				"-t				Show time\n"
				"-a				Show hidden\n"
				"-l				Show as list\n"
				"-p				Show permissions\n"
		);
	} else if (strcmp(args[1], "environment") == 0) {
		println("Built-in function `sgls`\n");
		printf(
			"get <VAR>                   - get value of var (ex. get USER)\n"
			"set <VAR> <VALUE>           - set value for var (ex. set GANG SHELL)\n"
		);
	}

	return 1;
}

/**
 * @brief      Function for exit and kill all tasks
 *
 * @return     int Status code
 */
int quit_from_shell(char** args) {
	bg_task* bt;

	signal(SIGCHLD, SIG_IGN);

	if (!tasks_structure.foreground_task.is_finished) {
		kill_foreground_task();
	}

	for (size_t i = 0; i < tasks_structure.cursor; i++) {
		bt = &tasks_structure.background_task[i];

		if (bt->is_finished == 0) {
			kill(bt->pid, SIGTERM);
		}

		free(bt->command);
	}

	return 0;
}

/**
 * @brief      Function for print command history
 *
 * @return     int Status code
 */
int print_history(char** args) {
	if (args[1] != NULL && strcmp(args[1], "clear") == 0) {
		printf("Clean...\n");
		ic_history_clear();
		return 1;
	}

	FILE* file = fopen(get_history_file_path(), "r");
	if (file == NULL) {
		perror("fopen");
		return 1;
	}

	char line[256];
	int lineNumber = 1;

	while (fgets(line, sizeof(line), file)) {
		printf("%3d: %s", lineNumber, line);
		lineNumber++;
	}

	fclose(file);

	return 1;
}

/**
 * @brief      Reload shell (reload config)
 */
int reload_shell(char** args) {
	load_main_config();
	load_config_aliases();

	return 1;
}

/**
 * @brief      Execute alias
 *
 * @param      args  The arguments
 *
 * @return     status code
 */
int execute_alias(char **args) {
	for (int i = 0; i < num_aliases; i++) {
		if (strcmp(aliases[i].name, args[0]) == 0) {
			char cmd[strlen(aliases[i].command)];
			sprintf(cmd, "%s", aliases[i].command);
			printf("alias %s=%s\n\n", aliases[i].name, aliases[i].command);
			
			char** command_tokens = split_into_tokens(cmd);

			return execute(command_tokens);
		}
	}

	return 0;
}

/**
 * @brief      built-in basic util for work with env vars
 *
 * @param      args  The arguments
 *
 * @return     status code
 */
int environment(char** args) {
	if (args[1] == NULL || args[2] == NULL) {
		global_status_code = -1;
		println("Usage: environment <get> <var> OR environment <set> <var> <value>");
		return 1;
	}

	if (strcmp(args[1], "get") == 0) {
		printf("%s\n", get_environment_variable(args[2]));
		return 1;
	} else if (strcmp(args[1], "set") == 0 && args[2] != NULL && args[3] != NULL) {
		char* result = set_environment_variable(args[2], args[3]);
		printf("ENV: %s\n", result);
		return 1;
	}

	println("Usage: environment <get> <var> OR environment <set> <var> <value>");
	return 1;
}

/**
 * @brief      `echo -n` command analogue
 *
 * @param      args  The arguments
 *
 * @return     status code
 */
int echo_n(char **args) {
	for (int i = 1; args[i] != NULL; i++) {
		if (strncmp(args[i], "$", 1) == 0) {
			char* env_var = get_environment_variable(args[i] + 1);

			if (env_var != NULL) {
				printf("%s", env_var);
			} else {
				printf("Error: environment variable '%s' not found\n", args[i] + 1);
			}
		} else {
			printf("%s", args[i]);
			if (args[i + 1] != NULL) {
				printf(" ");
			}
		}
	}

	printf("\n");

	return 1;
}

/**
 * @brief      Function for execute command
 *
 * @param      args  The arguments
 *
 * @return     int Status code
 */
int execute(char** args) {
	struct { char *text, *hint; int arguments_count; int (*command)(char**); } commands[] = {
		//  Command            Minimal arguments count        Hint for command                       Pointer to command
		{.text="cd"				, .arguments_count=1, .hint="Change directory: cd <dir>", 		.command=&change_directory},
		{.text="exit"			, .arguments_count=0, .hint="Quit/exit from shell: exit", 		.command=&quit_from_shell},
		{.text="help"			, .arguments_count=0, .hint="Help command. Support args.", 		.command=&help},
		{.text="bg"				, .arguments_count=0, .hint="Print background tasks", 			.command=&bg_tasks},
		{.text="term"			, .arguments_count=1, .hint="Terminate background task", 		.command=&term_background_task},
		{.text="history"		, .arguments_count=0, .hint="Shows or clean command history", 	.command=&print_history},
		{.text="reload"			, .arguments_count=0, .hint="Reload shell configuration", 		.command=&reload_shell},
		{.text="shegang_config"	, .arguments_count=0, .hint="Configurate current session", 		.command=&shegang_config},
		{.text="sgls"			, .arguments_count=0, .hint="list: sgls <dir> -p|t|l|a", 		.command=&sgls},
		{.text="sglsblk"		, .arguments_count=0, .hint="lsblk command analogue", 			.command=&sglsblk},
		{.text="gapf"			, .arguments_count=1, .hint="cat analogue: gapf <file>", 		.command=&gapf},
		{.text="environment"	, .arguments_count=2, .hint="Env vars (view help environment)", .command=&environment},
		{.text="echon"			, .arguments_count=0, .hint="echo with env vars",				.command=&echo_n}
	};

	const int len = sizeof(commands) / sizeof(commands[0]);
	int status = 1;

	if (strcmp(args[0], "sghint") == 0) {
		for (int i = 0; i < len; ++i) {
			if (args[1] == NULL) {
				printf("Usage: sghint <built-in command>\n");
				return 1;
			} else if (strcmp(args[1], commands[i].text) == 0) {
				printf("%ssghint%s: %s\n", GRAY, RESET, commands[i].hint);
			}
		}

		return 1;
	}

	for (int i = 0; i < len; ++i) {
		if (strcmp(args[0], commands[i].text) == 0) {
			if (strcmp(commands[i].text, "exit") == 0) {
				commands[i].command(args);
			}
			if (commands[i].arguments_count > 0) {
				for (int j = 1; j < commands[i].arguments_count + 1; ++j) {
					if (args[j] == NULL) {
						printf("[Argument Count Error at %s:%d] %s\n", commands[i].text, j, commands[i].hint);
						global_status_code = -1;
						return 1;
					}
				}
			}

			return commands[i].command(args);
		}
	}

	if (execute_alias(args) == 0) {
		return launch_task(args);
	}

	return 1;
}
