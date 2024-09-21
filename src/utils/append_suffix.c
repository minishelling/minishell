#include "../../include/minishell.h"

// // TEST This function. Idea: make test directory equipped with libft.
t_ecode	append_suffix(char **str, char *suffix, bool duplicate)
{
	size_t	suffix_len;

	if (!*str || !suffix)
		return (NULL_STRING);
	suffix_len = ft_strlen(suffix);
	// printf("In append_suffix: str[suffix_len]: %s\n", str[suffix_len]);
	if (!ft_strncmp(str[suffix_len], suffix, suffix_len) && !duplicate)
	{
		// printf("in append_suffix, returning success\n");
		return (SUCCESS);
	}
	*str = ft_strjoin_fs1(str, suffix);
	if (!*str)
		return (MALLOC_ERROR);
	else return (SUCCESS);
}