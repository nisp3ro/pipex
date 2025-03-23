#include "pipex.h"

/**
 * @brief Counts the number of words in a string based on a given charset.
 *
 * This function counts words in the input string while treating substrings
 * enclosed in braces '{}' as single words, regardless of the delimiters in the charset.
 *
 * @param str The input string to analyze.
 * @param charset A string of delimiter characters.
 * @return int The number of words found.
 */
static int count_words(char *str, char *charset)
{
	int i;
	int words;
	int in_braces;

	i = -1;
	words = 0;
	in_braces = 0;
	while (str[++i] != '\0')
	{
		if (str[i] == '{')
		{
			in_braces = 1;
			while (str[i] != '}' && str[i + 1] != '\0')
				i++;
			if (str[i] == '}')
			{
				words++;
				in_braces = 0;
			}
		}
		else if (!ft_strchr(charset, str[i]) && (ft_strchr(charset, str[i + 1])
				|| str[i + 1] == '\0') && !in_braces)
			words++;
	}
	return (words);
}

/**
 * @brief Copies a word from the source string to the destination buffer.
 *
 * This function copies characters from the source string into the destination buffer
 * until a delimiter from charset is encountered. It also handles words enclosed in braces '{}'
 * as a single unit.
 *
 * @param dest The destination buffer to store the copied word.
 * @param from The source string to copy from.
 * @param charset A string of delimiter characters.
 */
static void copy_word(char *dest, char *from, char *charset)
{
	int i;
	int j;

	i = 0;
	j = 0;
	while (from[i] != '\0' && !ft_strchr(charset, from[i]))
	{
		if (from[i] == '{')
		{
			while (from[i] != '}' && from[i] != '\0')
				dest[j++] = from[i++];
			if (from[i] == '}')
				dest[j++] = from[i++];
		}
		else
			dest[j++] = from[i++];
	}
	dest[j] = '\0';
}

/**
 * @brief Allocates memory for a single word from the input string.
 *
 * This function determines the length of the next word in the string (handling brace-enclosed words)
 * and allocates memory for it.
 *
 * @param split The array of strings where the word will be stored.
 * @param str The input string from which to allocate the word.
 * @param charset A string of delimiter characters.
 * @param word_index Pointer to the current word index in the split array.
 * @return int The length of the word allocated, or 0 if allocation failed.
 */
static int allocate_word(char **split, char *str, char *charset, int *word_index)
{
	int j;

	j = 0;
	while (!ft_strchr(charset, str[j]) && str[j] != '\0')
	{
		if (str[j] == '{')
		{
			while (str[j] != '}' && str[j] != '\0')
				j++;
			if (str[j] != '}')
				break ;
		}
		j++;
	}
	split[*word_index] = (char *)malloc(sizeof(char) * (j + 1));
	if (!split[*word_index])
		return (0);
	return (j);
}

/**
 * @brief Allocates memory for each word and splits the input string.
 *
 * This function iterates over the input string, allocates memory for each word found,
 * and fills the split array with the individual words.
 *
 * @param split The array of strings to fill with words.
 * @param str The input string to split.
 * @param charset A string of delimiter characters.
 */
static void allocate_and_split(char **split, char *str, char *charset)
{
	int i;
	int word_index;
	int j;

	i = 0;
	word_index = 0;
	while (str[i] != '\0')
	{
		if (ft_strchr(charset, str[i]))
			i++;
		else
		{
			j = allocate_word(split, str + i, charset, &word_index);
			if (!j)
				break ;
			copy_word(split[word_index], str + i, charset);
			i += j;
			word_index++;
		}
	}
}

/**
 * @brief Splits a string into an array of words based on a set of delimiters.
 *
 * This function splits the input string into words using the provided charset as delimiters.
 * Words enclosed in braces '{}' are treated as a single word.
 *
 * @param str The input string to split.
 * @param charset A string of delimiter characters.
 * @return char** A dynamically allocated, NULL-terminated array of strings containing the words.
 */
char **pipex_split(char *str, char *charset)
{
	char **res;
	int words;

	words = count_words(str, charset);
	res = (char **)malloc(sizeof(char *) * (words + 1));
	if (!res)
		return (NULL);
	res[words] = NULL;
	allocate_and_split(res, str, charset);
	return (res);
}
