#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

int setenv(const char* envname, const char*envval, int overwrite);

char *get_environment_variable(char* variable_name);
char *set_environment_variable(char* variable_name, const char* variable_value);

#endif // ENVIRONMENT_H