#include "../../include/minishell.h"

void	ft_free_3d(void ****arr)
{
	int	i;
	int	j;

	if (!arr || !*arr)
		return ;
	i = 0;
	while ((*arr)[i] != NULL)
	{
		j = 0;
		while ((*arr)[i][j] != NULL)
		{
			free((*arr)[i][j]);
			j++;
		}
		free((*arr)[i]);
		i++;
	}
	free(*arr);
	*arr = NULL;
	return ;
}
