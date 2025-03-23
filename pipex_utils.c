#include "pipex.h"

/**
 * @brief Checks that the environment variables are set and contain PATH.
 *
 * This function verifies that the provided environment variables are not empty
 * and that one of them contains the "PATH" variable. If either condition fails,
 * an error message is printed and the program exits.
 *
 * @param envp The array of environment variable strings.
 */
void check_env(char **envp)
{
	int	i;

	i = 0;
	if (!envp[i])
	{
		ft_putstr_fd("\033[31mError: No env\n\e[0m", 2);
		exit(EXIT_FAILURE);
	}
	while (envp[i] && ft_strnstr(envp[i], "PATH", 5) == 0)
		i++;
	if (!envp[i])
	{
		ft_putstr_fd("\033[31mError: No PATH in env\n\e[0m", 2);
		exit(EXIT_FAILURE);
	}
}

/**
 * @brief Frees a dynamically allocated array of strings.
 *
 * This function iterates over the array of strings, freeing each element,
 * and finally frees the array itself.
 *
 * @param tofree The null-terminated array of strings to free.
 */
void liberator(char **tofree)
{
	int	i;

	i = -1;
	while (tofree[++i])
		free(tofree[i]);
	free(tofree);
}

/**
 * @brief Finds the full path of a command by searching in the PATH directories.
 *
 * This function searches for the specified command by iterating over the directories
 * listed in the PATH environment variable. If the command is found (i.e., it exists
 * and is accessible), the full path to the command is returned. Otherwise, NULL is returned.
 *
 * @param cmd The command name to locate.
 * @param envp The array of environment variable strings.
 * @return A string containing the full path of the command if found, otherwise NULL.
 */
char *cmd_finder(char *cmd, char **envp)
{
	char	**paths;
	char	*path;
	int		i;
	char	*part_path;

	i = 0;
	while (ft_strnstr(envp[i], "PATH", 5) == 0)
		i++;
	paths = ft_split(envp[i] + 5, ':');
	i = 0;
	while (paths[i])
	{
		part_path = ft_strjoin(paths[i], "/");
		path = ft_strjoin(part_path, cmd);
		free(part_path);
		if (access(path, F_OK) == 0)
			return (liberator(paths), path);
		free(path);
		i++;
	}
	liberator(paths);
	return (0);
}

/**
 * @brief Prints an error message and exits the program.
 *
 * This function uses perror to display the last system error message and exits
 * the program with a failure status.
 */
void error(void)
{
	perror("\033[31mError\e[0m");
	exit(EXIT_FAILURE);
}

/**
 * @brief Executes a command using execve.
 *
 * This function splits the command string into an array of arguments using pipex_split,
 * finds the full path of the command using cmd_finder, and then executes the command
 * with execve. If the command is not found or execution fails, it frees allocated memory,
 * prints an error message, and exits the program.
 *
 * @param argv The command string to execute.
 * @param envp The array of environment variable strings.
 */
void cmd_exec(char *argv, char **envp)
{
	char	**cmd;
	int		i;
	char	*path;

	i = -1;
	cmd = pipex_split(argv, " \"\'");
	path = cmd_finder(cmd[0], envp);
	if (!path)
	{
		while (cmd[++i])
			free(cmd[i]);
		free(cmd);
		error();
	}
	if (execve(path, cmd, envp) == -1)
	{
		while (cmd[++i])
			free(cmd[i]);
		free(cmd);
		free(path);
		error();
	}
}
