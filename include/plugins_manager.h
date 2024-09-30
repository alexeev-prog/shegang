#ifndef PLUGINS_MANAGER_H
#define PLUGINS_MANAGER_H

typedef struct {
	char* name;
	char* description;
	char* author;
	char* license;
} plugin_metadata_t;

typedef struct {
	int (*init)(void);
	int (*mainloop)(char* input);
	int (*deinit)(void);
} plugin_functions_t;

typedef struct {
	plugin_metadata_t metadata;
	plugin_functions_t functions;
} plugin_t;

int load_plugin(const char* plugin_path, plugin_t* plugin);
int unload_plugin(plugin_t* plugin);
int execute_plugin(plugin_t* plugin, char* input);
int print_plugin_info(plugin_t* plugin);

#endif // PLUGINS_MANAGER_H
