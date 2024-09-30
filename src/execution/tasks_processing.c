/**
 * @title SheGang Linux Shell | Tasks Processing (POSIX)
 * @filename tasks_processing.c
 * @brief Module with tasks management and background/foreground tasks structures
 * @authors Alexeev Bronislav
 * @license MIT License
 **/
#include <wait.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include "colors.h"
#include "executor.h"
#include "autocomplete.h"
#include "fuzzy.h"

// Functions definitions
int kill(pid_t pid, int);

extern int global_status_code;

/**
 * Struct background task
 *
 * @param      pid_t  Task PID
 * @param      int    is_finished Boolean value (is finished task)
 * @param      char*  timestamp Timestamp of task starting
 * @param      char*  command Command of task
 */
struct background_task_t {
	pid_t pid;
	int is_finished;
	char* timestamp;
	char* command;
};
typedef struct background_task_t bg_task;

/**
 * Struct foreground task
 *
 * @param      pid_t  Task PID
 * @param      int    is_finished Boolean value (is finished task)
 */
struct foreground_task_t {
	pid_t pid;
	int is_finished;
};
typedef struct foreground_task_t fg_task;

/**
 * Struct for all tasks
 *
 * @param      fg_task   foreground_task Foreground Task Struct
 * @param      bg_task*  background_task Background Task Struct pointer
 * @param      size_t    cursor Cursor
 * @param      size_t    capacity Task Capacity
 */
struct tasks_t {
	fg_task foreground_task;
	bg_task* background_task;
	size_t cursor;
	size_t capacity;
};
typedef struct tasks_t tasks;

/**
 * tasks_structure
 @brief      global tasks structure variable
*/
tasks tasks_structure = {
	.foreground_task = {
		.pid = -1,
		.is_finished = 1
	},
	.background_task = 0,
	.cursor = 0,
	.capacity = 0
};

/**
 * @brief      function for set foreground task in tasks_structure
 *
 * @param      pid   pid PID of process
 */
void set_foreground_task(pid_t pid) {
	tasks_structure.foreground_task.pid = pid;
	tasks_structure.foreground_task.is_finished = 0;
}

/**
 * @brief      function for add new background task
 *
 * @param      pid   pid PID of process
 * @param      name  The name
 * @param      char*  name Name of task
 *
 * @return     int
 */
int add_background_task(pid_t pid, char* name) {
	bg_task* bt;

	if (tasks_structure.cursor >= tasks_structure.capacity) {
		tasks_structure.capacity = tasks_structure.capacity * 2 + 1;
		tasks_structure.background_task = (bg_task*)realloc(tasks_structure.background_task, sizeof(bg_task) * tasks_structure.capacity);

		if (tasks_structure.background_task == 0 || tasks_structure.background_task == NULL) {
			print_message("Couldn't reallocate buffer for background tasks!", ERROR);
			return -1;
		}
	}

	printf("[%zu] task started\n", tasks_structure.cursor);

	bt = &tasks_structure.background_task[tasks_structure.cursor];

	bt->pid = pid;
	bt->is_finished = 0;

	time_t timestamp = time(NULL);
	bt->timestamp = ctime(&timestamp);

	bt->command = name;

	tasks_structure.cursor += 1;

	return 1;
}

/**
 * kill_foreground_task

 @brief      Function for kill foreground task
*/
void kill_foreground_task(void) {
	if (tasks_structure.foreground_task.pid != -1) {
		kill(tasks_structure.foreground_task.pid, SIGTERM);
		tasks_structure.foreground_task.is_finished = 1;

		printf("\n");
	}
}

/**
 * @brief      Terminate background task
 *
 * @param      args  The arguments
 * @param      char**  args Arguments of background task
 *
 * @return     int Result code
 */
int term_background_task(char** args) {
	char* idx_str;
	int proc_idx = 0;

	if (args[1] == NULL) {
		print_message("No process index to stop", ERROR);
	} else {
		idx_str = args[1];

		while (*idx_str >= '0' && *idx_str <= '9') {
			proc_idx = (proc_idx * 10) + ((*idx_str) - '0');

			idx_str += 1;
		}

		if (*idx_str != '\0' || (size_t)proc_idx >= tasks_structure.cursor) {
			print_message("Incorrect background process index!", ERROR);
		} else if (tasks_structure.background_task[proc_idx].is_finished == 0) {
			kill(tasks_structure.background_task[proc_idx].pid, SIGTERM);
		}
	}

	return 1;
}

/**
 * @brief      function for check is background task
 *
 * @param      args  The arguments
 * @param      char**  args Argument for task
 *
 * @return     int (bool) 1 if true, 0 is false
 */
int is_background_task(char** args) {
	int last_arg_id = 0;

	while (args[last_arg_id + 1] != NULL) {
		last_arg_id++;
	}

	if (strcmp(args[last_arg_id], "&") == 0) {
		args[last_arg_id] = NULL;

		return 1;
	}

	return 0;
}

/**
 * @brief      Fuzzy search valid command
 *
 * @param      args  The arguments
 */
void fuzzy_search_valid_command(char** args) {
	int *score = NULL;
	size_t score_len = 0;
	int max_score = 0;
	const char *text = "N/A";
	const char** apps = get_sys_binaries();

	for (size_t i = 0; i < sizeof(apps)/sizeof(apps[0]); i++) {
		int *score_tmp;
		size_t score_len_tmp;
		
		int total_score = fuzzy_search(apps[i], args[0], true, &score_tmp, &score_len_tmp);

		if (total_score > max_score) {
			if (score) free(score);
			
			score = score_tmp;
			score_len = score_len_tmp;
			text = apps[i];

			max_score = total_score;
		} else {
			free(score_tmp);
		}
	}

	if (max_score > 0)
		printf("Did you mean the command %s%s%s?\n", BOLD, text, RESET);
}

/**
 * @brief      function for create and launch task
 *
 * @param      args  The arguments
 * @param      char**  args Arguments array
 *
 * @return     status code
 */
int launch_task(char** args) {
	pid_t pid;
	int background = is_background_task(args);

	pid = fork();

	if (pid < 0) {
		print_message("Couldn't create child process!", ERROR);
	} else if (pid == 0) {

		if (execvp(args[0], args) != 0) {
			print_message("Couldn't execute unknown command!", ERROR);
			fuzzy_search_valid_command(args);
			global_status_code = -1;
		}
		
		exit(-1);
	} else {
		if (background == 1) {
			if (add_background_task(pid, args[0]) == -1) {
				quit_from_shell(args);
			}
		} else {
			set_foreground_task(pid);

			if (waitpid(pid, NULL, 0) == -1) {
				if (errno != EINTR) {
					print_message("Couldn't track the completion of the process", WARNING);
					global_status_code = -1;
				}
			}
		}
	}

	return 1;
}

/**
 * @brief      Function for marking ended tasks
 */
void mark_ended_task(void) {
	bg_task* bt;

	pid_t pid = waitpid(-1, NULL, 0);

	if (pid == tasks_structure.foreground_task.pid) {
		tasks_structure.foreground_task.is_finished = 1;
	} else {
		for (size_t i = 0; i < tasks_structure.cursor; i++) {
			bt = &tasks_structure.background_task[i];

			if (bt->pid == pid) {
				printf("Task %zu is finished\n", i);

				bt->is_finished = 1;

				break;
			}
		}
	}
}
