/**
 * Example plugin for Shegang shell
 * by alxvdev
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

// Metadata
typedef struct {
	char* name;
	char* description;
	char* author;
	char* license;
} plugin_metadata_t;

// Init - before shell mainloop
int plugin_init(void) {
	printf("Plugin Pipegang has been initialized. !Pipegang plugin is not ready to use!\n");
	return 1;
}

/**
 * @brief      Work with user input in shell mainloop
 *
 * @param      input  The input
 *
 * @return     status code
 */
int plugin_mainloop(char* input) {
	printf("INPUT: %s\n\n", input);

	return 0;
}

// Deinit - after exit from shell
int plugin_deinit(void) {
	printf("Deinitialize Pipegang plugin\n");

	return 0;
}

// Get plugin metadata;
void get_plugin_metadata(plugin_metadata_t* metadata) {
	metadata->name = "Pipegang";
	metadata->description = "This is example plugin for SheGang";
	metadata->author = "Alexeev Bronislav";
	metadata->license = "MIT License";
}
