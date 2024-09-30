/**
 * @title SheGang Linux Shell | Basic built-in utils
 * @filename builtin/base.c
 * @brief Module with base built-in utils
 * @authors Alexeev Bronislav
 * @license MIT License
 **/
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include "colors.h"
#include "tasks_processing.h"
#include "config.h"

extern tasks tasks_structure;
extern char* username_color;
extern char* pwd_color;
extern char* curr_time_color;
extern int global_status_code;

/**
 * @brief Function for change working directory
 * 
 * @param char** args Command arguments
 * 
 * @return int Status code
 **/
int change_directory(char** args) {
	if (args[1] == NULL) {
		print_message("Expected argument for \"cd\" command", ERROR);
	} else if (chdir(args[1]) != 0) {
		global_status_code = -1;
		print_message("Couldn't change directory", ERROR);
	}

	return 1;
}

/**
 * @brief Function for print background tasks
 * 
 * @return int Status code
 **/
int bg_tasks(char** args) {
	bg_task* bt;

	for (size_t i = 0; i < tasks_structure.cursor; i++) {
		bt = &tasks_structure.background_task[i];

		printf(
			"[%zu]%s command; %s%s;%s pid: %s%d; %s"
			"state: %s%s;%s timestamp: %s%s", i,
			MAGENTA, RESET, bt->command,
			MAGENTA, RESET, bt->pid,
			MAGENTA, RESET, bt->is_finished ? "is_finished" : "active",
			MAGENTA, RESET, bt->timestamp
		);
	}

	return 1;
}
