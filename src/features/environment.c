/**
 * @title SheGang Linux Shell | Environment
 * @filename environment.c
 * @brief Module with setenv and getenv POSIX APIs to set and get environment variables
 * @authors Alexeev Bronislav
 * @license MIT License
 **/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int setenv(const char* envname, const char*envval, int overwrite);

/**
 * @brief      Gets the environment variable.
 *
 * @param[in]  variable_name  The variable name
 *
 * @return     The environment variable.
 */
char *get_environment_variable(char* variable_name) {
	char* variable_value = getenv(variable_name);

	if (variable_value == NULL)
		return NULL;

	return variable_value;
}

/**
 * @brief      Sets the environment variable.
 *
 * @param[in]  variable_name   The variable name
 * @param[in]  variable_value  The variable value
 *
 * @return     string stauts
 */
char* set_environment_variable(char* variable_name, const char* variable_value) {
	int status = setenv(variable_name, variable_value, 1);

	if (status == 1) {
		return "ERROR";
	} else {
		return "SUCCESS";
	}
}
