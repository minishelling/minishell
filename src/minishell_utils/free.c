#include "../../include/free.h"

void	ft_free(void **var)
{
	if (!var || !*var)
		return ;
	free(*var);
	*var = NULL;
	return ;
}
