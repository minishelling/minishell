#include "../../include/minishell.h"

/**
 * @brief Returns the concatenation of s1 and s2,
 * while also freeing the first string in the process.
 * 
 * @param s1 A pointer to the first string.
 * @param s2 The second string.
 * 
 * @return If the first string is NULL, or there's a malloc failure
 * it returns NULL and in the latter case it prints an error message.
 * Otherwise it returns a new string that is the product of
 * concatenating s1 and s2.
 */
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
	dst = ft_calloc(ls1 + ls2 + 1, sizeof(char));
	if (!dst)
	{
		handle_perror("ft_strjoin_fs1");
		ft_free((void **) s1);
		return (NULL);
	}
	ft_strlcpy(dst, *s1, (ls1 + 1));
	ft_strlcat(dst, s2, (ls1 + ls2 + 1));
	ft_free((void **) s1);
	return (dst);
}

/**
 * @brief Duplicates the string src into dest,
 * freeing dest if it held any value beforehand.
 * 
 * @param dest A pointer to the destination string.
 * @param src The string whose value we want to duplicate.
 * 
 * @return On malloc failure it prints an error message
 * and returns an error code. On success it returns SUCCESS.
 */
int	safe_assign_str(char **dest, const char *src)
{
	if (*dest)
	{
		free(*dest);
		*dest = NULL;
	}
	if (src)
	{
		*dest = ft_strdup(src);
		if (*dest == NULL)
			return (handle_perror("safe_assign_str"), ERR_MEM);
	}
	else
		(*dest) = NULL;
	return (SUCCESS);
}
