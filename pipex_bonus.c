#include "pipex.h"

/**
 * @brief Creates a pipe, forks a process, and executes a command.
 *
 * This function creates a pipe and forks a child process. In the child process,
 * it redirects the standard output to the write end of the pipe and executes the given command.
 * The parent process redirects the standard input to the read end of the pipe and waits for the child.
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
 * @brief Reads from standard input and writes to a file descriptor until a limiter is reached.
 *
 * This function reads lines from the standard input using get_next_line() and writes each line to the write end
 * of the provided pipe file descriptor until a line matches the specified limiter.
 *
 * @param limiter The string that, when matched, stops further reading.
 * @param fd An array containing the file descriptors of the pipe.
 */
void read_n_write(char *limiter, int *fd)
{
	char	*line;

	line = get_next_line(STDIN_FILENO);
	while (line)
	{
		if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0)
		{
			free(line);
			break ;
		}
		write(fd[1], line, ft_strlen(line));
		free(line);
		line = get_next_line(STDIN_FILENO);
	}
}

/**
 * @brief Handles here-document input.
 *
 * This function creates a pipe and forks a child process to read input from standard input.
 * The child process writes the input to the pipe until the limiter is encountered.
 * The parent process then redirects its standard input to the read end of the pipe and waits for the child.
 *
 * @param limiter The string used to delimit the here-document input.
 */
void here_doc(char *limiter)
{
	pid_t	reader;
	int		fd[2];

	if (pipe(fd) == -1)
		error();
	reader = fork();
	if (reader == -1)
		error();
	if (reader == 0)
	{
		close(fd[0]);
		read_n_write(limiter, fd);
		close(fd[1]);
		exit(EXIT_SUCCESS);
	}
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	waitpid(reader, NULL, 0);
}

/**
 * @brief Selects the processing mode based on the command line arguments.
 *
 * Depending on whether "here_doc" is specified as the first argument,
 * this function either sets up a here-document or opens the input file.
 * It then iterates through the commands (except the last one) and calls fork_caller
 * to execute each command in sequence.
 *
 * @param argc The number of command line arguments.
 * @param argv The array of command line argument strings.
 * @param envp The environment variables.
 */
void process_selector(int argc, char **argv, char **envp)
{
	int	i;
	int	f_in;

	if (ft_strncmp(argv[1], "here_doc", 9) == 0)
	{
		if (argc < 6)
			usage();
		i = 3;
		here_doc(argv[2]);
	}
	else
	{
		i = 2;
		f_in = open_file(argv[1], 2);
		dup2(f_in, STDIN_FILENO);
		close(f_in);
	}
	while (i < argc - 2)
		fork_caller(argv[i++], envp);
}

/**
 * @brief Main entry point for the Pipex program.
 *
 * This function validates the environment, processes the command line arguments,
 * sets up input redirection (including handling here-document if specified),
 * and executes the final command with output redirection.
 *
 * @param argc The number of command line arguments.
 * @param argv The array of command line argument strings.
 * @param envp The environment variables.
 * @return int Returns EXIT_SUCCESS if the program completes successfully.
 */
int main(int argc, char **argv, char **envp)
{
	int	f_out;
	int pid;

	check_env(envp);
	if (argc >= 5)
	{
		process_selector(argc, argv, envp);
		f_out = open_out(argc, argv);
		pid = fork();
		if (pid == 0)
		{
			dup2(f_out, STDOUT_FILENO);
			close(f_out);
			cmd_exec(argv[argc -2], envp);
		}
		waitpid(pid, NULL, 0);
		close(f_out);
	}
	else
		usage();
}
