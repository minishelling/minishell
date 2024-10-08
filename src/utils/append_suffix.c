#include "../../include/minishell.h"

t_ecode	append_suffix(char **str, char *suffix, bool duplicate)
{
	char	*new_str;
	size_t	suffix_len;
	size_t	str_len;

	if (!*str)
		return (NULL_STRING);
	if (!suffix)
		return (SUCCESS);
	suffix_len = ft_strlen(suffix);
	str_len = ft_strlen(*str);
	new_str = ft_strdup(*str);
	if (!new_str)
		return (MALLOC_ERROR);
	if (!ft_strncmp(&new_str[str_len - suffix_len], suffix, suffix_len) && !duplicate)
		return (SUCCESS);
	ft_free((void **) str);
	new_str = ft_strjoin_fs1(&new_str, suffix);
	if (!new_str)
		return (MALLOC_ERROR);
	*str = ft_strdup(new_str);
	ft_free((void **) &new_str);
	if (!*str)
		return (MALLOC_ERROR);
	return (SUCCESS);
}
