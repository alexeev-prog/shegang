/**
 * @title SheGang Linux Shell | Plugins Manager
 * @filename plugins_manager.c
 * @brief Module with plugins manager and uploader
 * @authors Alexeev Bronislav
 * @license MIT License
 **/
#include "dlfcn.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "colors.h"
#include "plugins_manager.h"

/**
 * @brief      Loads a plugin.
 *
 * @param[in]  plugin_path  The plugin path
 * @param      plugin       The plugin
 *
 * @return     Status code
 */
int load_plugin(const char* plugin_path, plugin_t* plugin) {
	void* handle = dlopen(plugin_path, RTLD_LAZY);

	if (!handle) {
		fprintf(stderr, "%sError occured when loading plugin: %s%s\n", RED, dlerror(), RESET);
		return -1;
	}

	plugin->functions.init = dlsym(handle, "plugin_init");
	plugin->functions.mainloop = dlsym(handle, "plugin_mainloop");
	plugin->functions.deinit = dlsym(handle, "plugin_deinit");

	if (!plugin->functions.init ||
		!plugin->functions.mainloop ||
		!plugin->functions.deinit) {
		fprintf(stderr, "%sError occured when get plugin functions:%s%s\n", RED, dlerror(), RESET);
		dlclose(handle);

		return -1;
	}

	void (*get_plugin_metadata)(plugin_metadata_t*) = dlsym(handle, "get_plugin_metadata");

	if (!get_plugin_metadata) {
		fprintf(stderr, "%sError occured when get plugin metadata:%s%s\n", RED, dlerror(), RESET);
		dlclose(handle);

		return -1;
	}

	get_plugin_metadata(&plugin->metadata);
	printf("Plugin %s has been loaded\n", plugin->metadata.name);

	return 1;
}

/**
 * @brief      Unload plugin
 *
 * @param      plugin  The plugin
 *
 * @return     status code
 */
int unload_plugin(plugin_t* plugin) {
	printf("Plugin %s has been unloaded\n", plugin->metadata.name);

	return 0;
}

/**
 * @brief      Execute plugin mainloop
 *
 * @param      plugin  The plugin
 * @param      input   The input
 *
 * @return     Status code of plugin mainloop
 */
int execute_plugin(plugin_t* plugin, char* input) {
	return plugin->functions.mainloop(input);
}

/**
 * @brief      Prints a plugin information.
 *
 * @param      plugin  The plugin
 */
int print_plugin_info(plugin_t* plugin) {
	printf("Plugin: %s\n", plugin->metadata.name);
	printf("\tDescription: %s\n", plugin->metadata.description);
	printf("\tAuthor: %s\n", plugin->metadata.author);
	printf("\tLicense: %s\n", plugin->metadata.license);

	return 1;
}
