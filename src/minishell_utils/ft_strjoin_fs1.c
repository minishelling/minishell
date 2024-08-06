
#include "../../include/minishell.h"

// char	*ft_strjoin_fs1(char *s1, const char *s2)
// {
// 	char	*dst;
// 	size_t	len;
// 	int		i;
// 	int		j;

// 	if (!s1)
// 		return (NULL);
// 	len = ft_strlen(s1) + ft_strlen(s2);
// 	dst = malloc (sizeof(char) * (len + 1));
// 	if (!dst)
// 		return (ft_free((void **) &s1), NULL);
// 	i = 0;
// 	while (s1 && s1[i])
// 	{
// 		dst[i] = s1[i];
// 		i++;
// 	}
// 	j = 0;
// 	while (s2 && s2[j])
// 	{
// 		dst[i + j] = s2[j];
// 		j++;
// 	}
// 	dst[i + j] = '\0';
// 	return (ft_free((void **) &s1), dst);
// }

char	*ft_strjoin_fs1(char *s1, char const *s2)
{
	char	*dst;
	size_t	ls1;
	size_t	ls2;

	if (!s1)
		return (NULL);
	if (!s2)
		return (s1);
	ls1 = ft_strlen(s1);
	ls2 = ft_strlen(s2);
	dst = malloc(sizeof(char) * (ls1 + ls2 + 1));
	if (!dst)
		return (free(s1), NULL);
	ft_strlcpy(dst, s1, (ls1 + 1));
	ft_strlcat(dst, s2, (ls1 + ls2 + 1));
	free(s1);
	return (dst);
}

