/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtins.h                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/11 18:29:01 by lprieri       #+#    #+#                 */
/*   Updated: 2024/07/11 18:29:01 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "minishell.h"

typedef struct s_curpath
{
	char				*dir;
	struct s_curpath	*previous;
	struct s_curpath	*next;
}	t_curpath;

/*		CD		*/

//	CD

t_ecode		execute_cd(t_env *env, char *directory);
char		*cd_trim_curpath(char **curpath);

//	CURPATH

t_curpath	*curpath_new_node(char *dir, t_curpath *previous, t_curpath *next);
void		curpath_del_node(t_curpath **node);
void		curpath_del_list(t_curpath **head);
t_curpath	*curpath_get_last(t_curpath *head);
void		curpath_add_back(t_curpath **head, t_curpath *new);
void		curpath_del_last(t_curpath **head);
char		*curpath_concat(t_curpath *head);
int			curpath_check_access(char *curpath);

#endif