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

int	safe_assign_str(char **dest, const char *src)
{
	if (*dest)
	{
		//printf ("freeing %p and setting to NULL\n", *dest);
		free(*dest);
		*dest = NULL;
	}
	if (src)
	{
		*dest = strdup(src);
		if (*dest == NULL)
			return(ERR_MEM);
	}
	else
		(*dest) = NULL;
	return (SUCCESS);
}