#include "../../include/minishell.h"

/**
 * @brief Prints the strings in an array of strings.
 * 
 * @param arr The array of strings.
 * 
 * @return void
 */
void	ft_print_2d_arr(char **arr)
{
	int	i;

	if (!arr || !*arr)
	{
		return ;
	}
	i = 0;
	while (arr[i])
	{
		ft_printf("%s\n", arr[i]);
		i++;
	}
}

/**
 * @brief Prints the strings in an array
 * that contains arrays of strings.
 * 
 * @param arr The array of array of strings.
 * 
 * @return void
 */
void	ft_print_3d_arr(char ***arr)
{
	int	i;
	int	j;

	if (!arr || !*arr)
	{
		return ;
	}
	i = 0;
	while (arr[i])
	{
		j = 0;
		while (arr[j])
		{
			ft_printf("%s\n", arr[i][j]);
			j++;
		}
		i++;
	}
}
