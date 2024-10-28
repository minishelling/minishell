#include "../../include/minishell.h"

/**
 * @brief Frees a dynamically allocated 3D array and sets its pointer to NULL.
 *
 * This function safely frees memory allocated to a 3D array and sets its
 * main pointer to `NULL` to prevent dangling pointers. It takes a 
 * quadruple pointer representing a 3D array, checks that it’s valid, and 
 * performs the following actions:
 * - Iterates through each 2D array within the 3D array, freeing all dynamically
 *   allocated inner arrays.
 * - Frees each 2D array after all its elements are freed.
 * - Frees the main 3D array pointer itself and then sets it to `NULL`.
 *
 * @param[in,out] arr A quadruple pointer to the 3D array to be freed.
 *                    If the pointer is valid, each level of the array will
 *                    be freed, and the main pointer will be set to NULL.
 *
 * @note This function is designed for 3D arrays where all inner levels 
 *       (both 2D arrays and individual elements) have been dynamically 
 *       allocated.
 */
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
