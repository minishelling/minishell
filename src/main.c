/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/27 10:34:34 by lprieri       #+#    #+#                 */
/*   Updated: 2024/07/09 20:08:02 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main (int argc, char **argv, char **envp)
{
	t_env	*env_list;

	(void)argc;
	(void)argv;
	// ft_print_2d_arr(envp);
	// printf("%i\n", count_envp_vars(envp));
	env_list = NULL;
	env_init_list(&env_list, envp);
	env_print_list(env_list);
	env_free_list(&env_list);
	return (0);
}
