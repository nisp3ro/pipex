#ifndef PIPEX_H
# define PIPEX_H

# include "./libft/include/libft.h"
# include "./libft/include/get_next_line.h"
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>

//Mandatory
void	error(void);
void	cmd_exec(char *argv, char **envp);
void	check_env(char **envp);
char	*cmd_finder(char *cmd, char **envp);
char	**pipex_split(char *str, char *charset);

//Bonus
int		open_file(char *argv, int i);
void	usage(void);
int		open_out(int argc, char **argv);

#endif
