/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_split.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/11 13:13:21 by lprieri       #+#    #+#                 */
/*   Updated: 2024/02/06 20:05:08 by lisandro      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
* Function name: ft_split
* Description: Allocates (with malloc(3)) and returns an array of strings
*	obtained by splitting ’s’ using the character ’c’ as a delimiter.
*	The array must end with a NULL pointer.
* External functs.: malloc, free
* Return value: The array of new strings resulting from the split.
*	NULL if the allocation fails.
* Parameters:
*	char const *s: The string to be split.
*	char c: The delimiter character.
*/

#include "../libft.h"

static size_t	ft_count_words(char const *s, char c)
{
	int	i;
	int	counter;

	i = 0;
	counter = 0;
	while (s[i])
	{
		if (s[i] != c)
		{
			counter++;
			while (s[i] && s[i] != c)
				i++;
		}
		else
			i++;
	}
	return (counter);
}

static void	ft_free_arr(char ***arr)
{
	int	i;

	i = 0;
	while ((*arr)[i])
	{
		free((*arr)[i]);
		(*arr)[i] = NULL;
		i++;
	}
	free(*arr);
	*arr = NULL;
}

static void	ft_split_words(char **arr, const char *s, char c, int wordcount)
{
	int		i;
	int		j;
	int		k;

	i = 0;
	j = 0;
	k = 0;
	while (i < wordcount)
	{
		k = 0;
		while (s[j] && s[j] == c)
			j++;
		while (s[j] && s[j] != c)
		{
			j++;
			k++;
		}
		arr[i] = ft_substr(s, (j - k), k);
		if (!arr[i])
			return (ft_free_arr(&arr));
		i++;
	}
	arr[i] = NULL;
}

char	**ft_split(char const *s, char c)
{
	char	**arr;
	size_t	wordcount;

	if (!s)
		return (NULL);
	wordcount = ft_count_words(s, c);
	arr = (char **) malloc ((wordcount + 1) * sizeof(char *));
	if (!arr)
		return (NULL);
	ft_split_words(arr, s, c, wordcount);
	return (arr);
}

/*
* Test 1 - String with multiple separators in between words
*/

// int	main(void)
// {
// 	char	str1[] = " lalala   fiwhe ufqnidqb   dhquiwhdiq ";
// 	char	sep = ' ';
// 	char	**arr;
// 	int		i;

// 	arr = ft_split(str1, sep);
// 	if (arr == NULL)
// 	{
// 		printf("%s\n", "NULL");
// 		return (0);
// 	}
// 	i = 0;
// 	while (i < ft_count_words(str1, sep))
// 	{
// 		printf("%s\n", arr[i]);
// 		i++;
// 	}
// 	i = 0;
// 	while (i < ft_count_words(str1, sep))
// 	{
// 		free(arr[i]);
// 		i++;
// 	}
// 	free (arr);
// }

/*
* Test 2 - Empty string with not-null separator
*/

// int	main(void)
// {
// 	char	str1[] = "";
// 	char	sep = 'a';
// 	char	**arr;
// 	int		i;

// 	arr = ft_split(str1, sep);
// 	if (arr == NULL)
// 	{
// 		printf("%s\n", "NULL");
// 		return (0);
// 	}
// 	i = 0;
// 	while (i < ft_count_words(str1, sep))
// 	{
// 		printf("%s\n", arr[i]);
// 		i++;
// 	}
// 	i = 0;
// 	while (i < ft_count_words(str1, sep))
// 	{
// 		free(arr[i]);
// 		i++;
// 	}
// 	free (arr);
// }

/*
* Test 3 - String with words separated by a null terminator
*/

// int	main(void)
// {
// 	char	str1[] = "aaa\0bbbb\0fsdfse";
// 	char	sep = '\0';
// 	char	**arr;
// 	int		i;

// 	arr = ft_split(str1, sep);
// 	if (!arr)
// 		printf("%s\n", "NULL");
// 	else
// 	{
// 		i = 0;
// 		while (i < ft_count_words(str1, sep))
// 		{
// 			printf("%s\n", arr[i]);
// 			i++;
// 		}
// 		i = 0;
// 		while (i < ft_count_words(str1, sep))
// 		{
// 			free(arr[i]);
// 			i++;
// 		}
// 		free (arr);
// 	}
// }
