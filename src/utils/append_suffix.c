#include "../../include/minishell.h"

t_ecode	append_suffix(char **str, char *suffix, bool duplicate)
{
	size_t	suffix_len;
	size_t	str_len;

	fprintf(stderr, "Checkpoint 1 in append_suffix\n");
	if (!*str)
		return (NULL_STRING);
	fprintf(stderr, "Checkpoint 2 in append_suffix\n");
	if (!suffix)
		return (SUCCESS);
	fprintf(stderr, "Checkpoint 3 in append_suffix\n");
	suffix_len = ft_strlen(suffix);
	str_len = ft_strlen(*str);
	fprintf(stderr, "Compared str: %s\n", str[str_len - suffix_len]);
	if (!ft_strncmp(str[str_len - suffix_len], suffix, suffix_len) && !duplicate) // Is the string even properly compared?
		return (SUCCESS);
	fprintf(stderr, "Checkpoint 4 in append_suffix\n");
	*str = ft_strjoin_fs1(str, suffix);
	fprintf(stderr, "Checkpoint 5 in append_suffix\n");
	if (!*str)
		return (MALLOC_ERROR);
	return (SUCCESS);
}
