/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/27 10:34:34 by lprieri       #+#    #+#                 */
/*   Updated: 2024/07/25 20:22:47 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main (int argc, char **argv, char **envp)
{
	t_env		*env_list;
	t_cd_ecode	cd_status;
	char		*cwd;

	(void)argc;
	env_list = NULL;
	env_init_list(&env_list, envp);
	// env_print_list(env_list);
	cwd = getcwd(NULL, PATH_MAX);
	printf("CWD before: %s\n", cwd);
	cd_status = execute_cd(env_list, argv[1]);
	printf("CD STATUS: %i\n", cd_status);
	cwd = getcwd(NULL, PATH_MAX);
	printf("CWD after: %s\n", cwd);
	env_free_list(&env_list);
	return (0);
}