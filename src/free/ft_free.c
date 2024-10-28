#include "../../include/minishell.h"

/**
 * @brief Frees a pointer and sets it to NULL.
 *
 * This function safely frees memory allocated to a pointer and sets it to
 * `NULL` to prevent dangling pointers. It accepts a double pointer as an 
 * argument and performs the following checks:
 * - If the provided pointer is NULL or if it already points to NULL, the 
 *   function exits without any action.
 * - If the pointer is valid, the function frees the allocated memory and 
 *   sets the pointer to NULL.
 *
 * @param[in,out] var A double pointer to the memory to be freed.
 *                    If the pointer is not NULL and points to valid memory,
 *                    this memory will be freed, and the pointer will be set 
 *                    to NULL.
 *
 * @note This function is intended to prevent memory leaks and dangling 
 *       pointers by clearing the pointer reference after freeing.
 */
void	ft_free(void **var)
{
	if (!var || !*var)
		return ;
	free(*var);
	*var = NULL;
	return ;
}
