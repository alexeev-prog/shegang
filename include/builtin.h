#ifndef BUILTIN_H
#define BUILTIN_H

#include <stdbool.h>

/* BASE BUILT-IN UTILS */
int change_directory(char** args);
int bg_tasks(char** args);


/* SHEGANG_CONFIG UTIL */
int shegang_config(char** args);
int set_value_for_var(char* var_name, char* value);


/* SGLS (LS) UTIL */
int sgls(char** args);


/* SGLSBLK (LSBLK) UTIL */
#define BUF_SIZE 1024
struct Disk {
	char name[BUF_SIZE];
	int maj_min;
	int rm;
	long long size;
	bool ro;
	char type[BUF_SIZE];
	char mountpoint[BUF_SIZE];
};
void print_disk(struct Disk disk);
int sglsblk(char** args);


/* GAPF (CAT) UTIL */
void printFileDetails(const char* filename);
void printFileType(const char* filename);
int gapf(char** args);


/* Nightswatch */
int nightwatch_newborn(char** args);

#endif // BUILTIN_H
