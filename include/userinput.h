#ifndef USERINPUT_H
#define USERINPUT_H

#define DEFAULT_BUFFER_SIZE 128
#define TOKENS_DELIMETERS " \t"

char* read_user_input(void);
char* fgets_input(void);
char** split_into_tokens(char* line);
void setup_isocline(void);
char* get_history_file_path(void);
char* r_line(char* filename, int line_n);
char** split_by_delims(char* line, const char* delim, int *num_tokens);

#endif // USERINPUT_H
