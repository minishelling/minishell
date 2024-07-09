/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/24 13:58:20 by lprieri       #+#    #+#                 */
/*   Updated: 2024/07/09 20:17:31 by lprieri       ########   odam.nl         */
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
# include "errors.h"

// typedef struct s_shell
// {
// 	t_env	*env_list;
// 	char	*pwd;
// 	int		pipefd[2];
// 	int		read_fd;
// 	int		write_fd;
	
// }	t_shell;

typedef struct s_env
{
	char			*keyvalue;
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef enum
{
	ENV_SUCCESS = 0,
	ENV_NULL,
	ENV_NEW_NODE_ERROR,
	ENV_GETKEYVALUE_ERROR,
	ENV_GETKEY_ERROR,
	ENV_GETVALUE_ERROR,
	ENV_COUNT
}	t_env_ecode;


/* ===== FUNCTIONS ===== */

/*	ENV	 */

void	env_free_list(t_env	**head);
t_env_ecode	env_init_list(t_env **head, char **envp);
void	env_print_list(t_env *head);
ssize_t	env_count_keys(char **envp);
t_env	*env_new_node(void);
char	*env_get_key(char *keyvalue);
char	*env_get_value(char *keyvalue);
t_env_ecode	env_copy_keyval(t_env **new_node, char *keyvalue);

/*	UTILS  */

// PRINT

void	ft_print_3d_arr(char ***arr);
void	ft_print_2d_arr(char **arr);

#endif