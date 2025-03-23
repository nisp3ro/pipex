#include "pipex.h"

/**
 * @brief Opens the output file with the appropriate mode based on the input mode.
 *
 * Depending on whether "here_doc" is specified as the first argument,
 * this function opens the output file either in append mode (for here_doc)
 * or in truncate mode.
 *
 * @param argc The number of command line arguments.
 * @param argv The array of command line argument strings.
 * @return int The file descriptor for the opened output file.
 */
int open_out(int argc, char **argv)
{
	int	f_out;

	if (ft_strncmp(argv[1], "here_doc", 8) == 0)
		f_out = open_file(argv[argc - 1], 0);
	else
		f_out = open_file(argv[argc - 1], 1);
	return (f_out);
}

/**
 * @brief Displays usage information and exits the program.
 *
 * This function prints an error message and example usage of the program,
 * then exits with EXIT_SUCCESS.
 */
void usage(void)
{
	ft_putstr_fd("\033[31mError: Bad argument\n\e[0m", 2);
	ft_putstr_fd("Ex: ./pipex <file1> <cmd1> <cmd2> <...> <file2>\n", 1);
	ft_putstr_fd("    ./pipex \"here_doc\" <LIMITER> <cmd> <cmd1> <...> <file>\n", 1);
	exit(EXIT_SUCCESS);
}

/**
 * @brief Opens a file with the specified mode.
 *
 * This function opens the file specified by the given argument in a mode determined by the parameter 'i':
 * - If i == 0: Opens the file in append mode (write-only, create if it doesn't exist, append).
 * - If i == 1: Opens the file in truncate mode (write-only, create if it doesn't exist, truncate).
 * - If i == 2: Opens the file in read-only mode.
 *
 * If the file cannot be opened, the function calls error() and exits.
 *
 * @param argv The file name to open.
 * @param i The mode indicator (0: append, 1: truncate, 2: read-only).
 * @return int The file descriptor of the opened file.
 */
int open_file(char *argv, int i)
{
	int	file;

	file = 0;
	if (i == 0)
		file = open(argv, O_WRONLY | O_CREAT | O_APPEND, 0777);
	else if (i == 1)
		file = open(argv, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	else if (i == 2)
		file = open(argv, O_RDONLY, 0777);
	if (file == -1)
		error();
	return (file);
}
