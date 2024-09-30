#ifndef TASKS_PROCESSING_H
#define TASKS_PROCESSING_H

#include <wait.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pwd.h>
#include <errno.h>
#include "../include/colors.h"

// Functions definitions
int kill(pid_t pid, int);
int quit(void);

/**
 * Struct background task
 * 
 * @param pid_t Task PID
 * @param int is_finished Boolean value (is finished task)
 * @param char* timestamp Timestamp of task starting
 * @param char* command Command of task 
 **/
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
 * @param pid_t Task PID
 * @param int is_finished Boolean value (is finished task)
 **/
struct foreground_task_t {
	pid_t pid;
	int is_finished;
};
typedef struct foreground_task_t fg_task;

/**
 * Struct for all tasks
 * 
 * @param fg_task foreground_task Foreground Task Struct
 * @param bg_task* background_task Background Task Struct pointer
 * @param size_t cursor Cursor
 * @param size_t capacity Task Capacity
 **/
struct tasks_t {
	fg_task foreground_task;
	bg_task* background_task;
	size_t cursor;
	size_t capacity;
};
typedef struct tasks_t tasks;

void set_foreground_task(pid_t pid);
int add_background_task(pid_t pid, char* name);
void kill_foreground_task();
int term_background_task(char** args);
int is_background_task(char** args);
void mark_ended_task();
int launch_task(char** args);

#endif // TASKS_PROCESSING_H
