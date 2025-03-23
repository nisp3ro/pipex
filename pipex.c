#include "pipex.h"

/**
 * @brief Forks a child process to execute a command with output redirection.
 *
 * This function forks a child process that redirects its standard output to the provided file descriptor,
 * then executes the specified command using cmd_exec(). The parent process waits for the child to finish.
 *
 * @param argv The command string to execute.
 * @param envp The environment variables.
 * @param f_out The file descriptor to which the command's output will be redirected.
 */
void last_fork(char *argv, char **envp, int f_out)
{
	int pid;

	pid = fork();
	if (pid == 0)
	{
		dup2(f_out, STDOUT_FILENO);
		close(f_out);
		cmd_exec(argv, envp);
	}
	waitpid(pid, NULL, 0);
}

/**
 * @brief Creates a pipe, forks a process, and executes a command.
 *
 * This function creates a pipe and forks a child process. In the child process, it redirects the
 * standard output to the write end of the pipe and executes the given command via cmd_exec().
 * The parent process redirects its standard input to the read end of the pipe and waits for the child process to finish.
 *
 * @param argv The command string to execute.
 * @param envp The environment variables.
 */
void fork_caller(char *argv, char **envp)
{
	pid_t	pid;
	int		fd[2];

	if (pipe(fd) == -1)
		error();
	pid = fork();
	if (pid == -1)
		error();
	if (pid == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		cmd_exec(argv, envp);
	}
	else
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		waitpid(pid, NULL, 0);
	}
}

/**
 * @brief Entry point of the Pipex program.
 *
 * The main function verifies the correct number of arguments, checks the environment,
 * and sets up the input and output file descriptors. It then forks processes to execute commands
 * in a pipeline, connecting the output of one command to the input of the next.
 * If the number of arguments is not exactly 5, an error message is displayed.
 *
 * @param argc The number of command line arguments.
 * @param argv The array of command line argument strings.
 * @param envp The environment variables.
 * @return int Returns EXIT_SUCCESS if the program completes successfully.
 */
int main(int argc, char *argv[], char *envp[])
{
	int	f_in;
	int	f_out;

	check_env(envp);
	if (argc == 5)
	{
		f_in = open(argv[1], O_RDONLY, 0777);
		if (f_in == -1)
			error();
		dup2(f_in, STDIN_FILENO);
		close(f_in);
		fork_caller(argv[2], envp);
		f_out = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0777);
		if (f_out == -1)
			error();
		last_fork(argv[3], envp, f_out);
		close(f_out);
	}
	else
	{
		ft_putstr_fd("\033[31mError: Bad arguments\n\e[0m", 2);
		ft_putstr_fd("Ex: ./pipex <file1> <cmd1> <cmd2> <file2>\n", 1);
	}
	return (0);
}
