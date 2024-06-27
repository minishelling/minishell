/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/27 10:34:34 by lprieri       #+#    #+#                 */
/*   Updated: 2024/06/27 10:52:34 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "include/minishell.h"

int	main (int argc, char **argv, char **envp)
{
	t_shell	shell;

	init_shell(&shell);
	(void)argc;
	(void)argv;
	(void)envp;
	char	*c = " ";
	printf("%i\n", c[0]);
	return (0);
}
