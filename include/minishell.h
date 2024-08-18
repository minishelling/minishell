/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/24 13:58:20 by lprieri       #+#    #+#                 */
/*   Updated: 2024/08/18 20:53:20 by lprieri       ########   odam.nl         */
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
# include <linux/limits.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <string.h>
# include <stddef.h>
# include "../libft/libft.h"
# include "builtins.h"
# include "env.h"
# include "errors.h"
# include "free.h"
# include "utils.h"

// typedef struct s_shell
// {
// 	t_env	*env_list;
// 	char	*cwd;
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