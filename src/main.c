/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/27 10:34:34 by lprieri       #+#    #+#                 */
/*   Updated: 2024/07/03 15:54:23 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main (int argc, char **argv, char **envp)
{
	t_env	*env_list;

	(void)argc;
	(void)argv;
	env_list = malloc(sizeof(env_list));
	// printf("%i\n", count_envp_vars(envp));
	// copy_envp(&env_list, envp);
	ft_print_2d_arr(envp);
	return (0);
}
