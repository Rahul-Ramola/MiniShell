#ifndef ENV_H
#define ENV_H

void expand_variables(char **args);
int handle_env_builtin(char **args);

#endif
