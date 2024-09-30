#ifndef AUTOCOMPLETE_H
#define AUTOCOMPLETE_H

#include "isocline.h"

static void word_completer(ic_completion_env_t* cenv, const char* word);

void completer(ic_completion_env_t* cenv, const char* input);
void highlighter(ic_highlight_env_t* henv, const char* input, void* arg);
const char **get_sys_binaries(void);

#endif // AUTOCOMPLETE_H
