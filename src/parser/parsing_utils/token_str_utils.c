#include "../../../include/minishell.h"

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
		if (!*dest)
			return (handle_perror("safe_assign_str"), ERR_MEM);
	}
	else
		(*dest) = NULL;
	return (SUCCESS);
}

size_t	ft_strcspn(const char *str, const char *reject)
{
	size_t	count;

	count = 0;
	while (str[count])
	{
		if (ft_strchr(reject, str[count]))
			break ;
		count++;
	}
	return (count);
}
