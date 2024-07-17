/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/24 13:58:20 by lprieri       #+#    #+#                 */
/*   Updated: 2024/07/17 15:46:26 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <signal.h>
# include <stddef.h>
# include <sys/wait.h>
# include <sys/time.h>
# include <errno.h>
# include <limits.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <string.h>
# include "../libft/libft.h"
# include "env.h"
# include "errors.h"
# include "free.h"

// typedef struct s_shell
// {
// 	t_env	*env_list;
// 	char	*pwd;
// 	int		pipefd[2];
// 	int		read_fd;
// 	int		write_fd;
	
// }	t_shell;




/* ===== FUNCTIONS ===== */

/*	ENV	 */

/*	UTILS  */

// PRINT

void	ft_print_3d_arr(char ***arr);
void	ft_print_2d_arr(char **arr);

#endif