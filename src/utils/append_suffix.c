#include "../../include/minishell.h"

// // TEST This function. Idea: make test directory equipped with libft.
t_ecode	append_suffix(char **str, char *suffix, bool duplicate)
{
	size_t	suffix_len;

	if (!*str)
		return (NULL_STRING);
	if (!suffix)
		return (SUCCESS);
	suffix_len = ft_strlen(suffix);
	if (!ft_strncmp(str[suffix_len], suffix, suffix_len) && !duplicate) // Is the string even properly compared?
		return (SUCCESS);
	*str = ft_strjoin_fs1(str, suffix);
	if (!*str)
		return (MALLOC_ERROR);
	else return (SUCCESS);
}