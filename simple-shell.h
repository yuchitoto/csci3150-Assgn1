#ifndef SIMPLE_SHELL_H_
#define SIMPLE_SHELL_H_

int main(void);
int shell_read_line(char *cmd_buf);
int get_line_args(char *line, char **args);

#endif //SIMPLE_SHELL_H_
