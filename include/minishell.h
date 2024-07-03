/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/24 13:58:20 by lprieri       #+#    #+#                 */
/*   Updated: 2024/07/03 15:53:40 by lprieri       ########   odam.nl         */
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


typedef enum e_metacharacter
{
	METACHAR_SPACE,
	METACHAR_TAB,
	METACHAR_NEWLINE,
	METACHAR_PIPE,
	METACHAR_AND,
	METACHAR_SEMICOLON,
	METACHAR_OPEN_PAREN,
	METACHAR_CLOSE_PAREN,
	METACHAR_GREATER,
	METACHAR_LESSER,
	METACHAR_COUNT
}	t_metacharacter;

typedef enum e_control_operator
{
	CO_NEWLINE,
	CO_LOGICAL_OR,
	CO_LOGICAL_AND,
	CO_BACKGROUND_EXECUTION,
	CO_COMMAND_SEPARATOR,
	CO_CASE_TERMINATOR,
	CO_FALLTHROUGH_CASE_TERMINATOR,
	CO_CASE_TERMINATOR_WITH_FALLTHROUGH,
	CO_PIPE,
	CO_PIPE_WITH_STANDARD_ERROR,
	CO_SUBSHELL,
	CO_COUNT
}	t_control_operator;

// FUNCTIONS

void	copy_envp(t_env **env_list, char **envp);
int	count_envp_vars(char **envp);
void	ft_print_3d_arr(char ***arr);
void	ft_print_2d_arr(char **arr);


#endif