#include "../../include/minishell.h"

char	*ft_strjoin_fs1(char **s1, char const *s2)
{
	char	*dst;
	size_t	ls1;
	size_t	ls2;

	if (!*s1)
		return (NULL);
	if (!s2)
		return (*s1);
	ls1 = ft_strlen(*s1);
	ls2 = ft_strlen(s2);
	dst = malloc(sizeof(char) * (ls1 + ls2 + 1));
	if (!dst)
		return (ft_free((void **) s1), NULL);
	ft_strlcpy(dst, *s1, (ls1 + 1));
	ft_strlcat(dst, s2, (ls1 + ls2 + 1));
	ft_free((void **) s1);
	return (dst);
}

int safe_assign_str(char **dest, const char *src)
{
	if (!dest || !*dest)
		printf ("no!\n");
	if (*dest)
	{
		free(*dest);
		*dest = NULL;
	}
	if (src)
	{
		*dest = ft_strdup(src);
		if (*dest == NULL)
			return (ERR_MEM);
	}
	else
		(*dest) = NULL;
	return (SUCCESS);
}


/**
 * @brief Custom implementation of the standard C library function `strcspn` 
 *        (string complement span). Calculates the length of the initial 
 *        segment of `str` that consists entirely of characters not found in `reject`.
 *
 * This function scans through the string `str` and counts the number
 * of characters until it encounters a character from the `reject` string.
 * It returns the length of the initial segment of `str` that does not contain
 * any characters from `reject`.
 *
 * @param str The string to be scanned.
 * @param reject The string containing the characters to reject.
 * @return The number of characters in the initial segment of `str` 
 *         that are not in `reject`.
 */
size_t ft_strcspn(const char *str, const char *reject)
{
	size_t count;

	count = 0;
	while (str[count] != '\0')
	{
		if (ft_strchr(reject, str[count]))
			break;
		count++;
	}
	return (count);
}
