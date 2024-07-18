/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cd.c                                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/11 18:28:31 by lprieri       #+#    #+#                 */
/*   Updated: 2024/07/11 18:28:31 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtins.h"
#include "../../include/minishell.h"

#define SUCCESS 1

typedef struct s_curpath
{
	char				*dir;
	struct s_curpath	*previous;
	struct s_curpath	*next;
}	t_curpath;

t_curpath	*curpath_new_node(char *dir, t_curpath *previous, t_curpath *next)
{
	t_curpath	*new_node;

	new_node = malloc(sizeof(t_curpath) + 1);
	if (!new_node)
		return (NULL);
	new_node->dir = ft_strdup(dir);
	new_node->previous = previous;
	new_node->next = next;
	return (new_node);
}

void	curpath_free_node(t_curpath **node)
{
	if (!*node)
		return ;
	if ((*node)->dir)
		free((*node)->dir);
	free(*node);
	*node = NULL;
	return ;
}

void	curpath_free_list(t_curpath **head)
{
	t_curpath	*iterator;
	t_curpath	*current;

	if (!*head && !(*head)->next)
		return ;
	iterator = *head;
	current = *head;
	while (iterator)
	{
		current = iterator;
		if (current != NULL)
			free(current->dir);
		iterator = current->next;
		free(current);
	}
}


t_ecode	execute_cd(t_env *env, char *directory)
{
	char	*curpath;
	char	*cwd;
	void	*temp;
	int		i;
	int		no_cdpath_flag;

	if (!directory)
	{
		temp = env_find_node("HOME");
		if (!temp || !temp.key)
			return (ERR_CD_NO_HOME);
		else
		{
			if (chdir(temp.key) == -1)
				return (ERR_CD_CHDIR_ERROR); //Errno is set.
		}	
	}
	else if (directory[0] == '/' || directory[0] == '.'
		|| (directory[0] == '.' && directory[1] == '.'))
		curpath = directory;
	else
	{
		temp = (t_env *) env_find_node("CDPATH");
		i = 0;
		while (i < env_count_values(env, "CDPATH")) //A value can be an empty string
		{
			curpath = ft_strdup(temp.values[i]);
			if (curpath == NULL && no_cdpath_flag == 0)
			{
				curpath = ft_strjoin("./", directory);
				if (access(curpath, F_OK) == -1)
				{
					ft_free(&curpath);
					i++;
					continue ;
				}
				if (chdir(curpath) == -1)
				{
					ft_free(&curpath);
					i++;
					continue ;
				}
				no_cdpath_flag = 1;
			}
			if (curpath[len -1] != '/')
				curpath = ft_strjoin_fs1(curpath, "/");
			curpath = ft_strjoin_fs1(curpath, directory);
			if (access(curpath, F_OK) == -1)
			{
				ft_free(&curpath);
				i++;
				continue ;
			}
			if (chdir(curpath) == -1)
			{
				ft_free(&curpath);
				i++;
				continue ;
			}
			i++;
		}
		curpath = directory;
		if (shell->pwd[len -1] != '/')
			shell->pwd = ft_strjoin_fs1(shell->pwd, "/"); //Protect mallocs
		curpath = cd_trim_curpath(shell, &curpath);
		// Check if (ft_strlen(curpath) + 1 > PATH_MAX)
		status = chdir(curpath);
		free(curpath);
		free(cwd);
		if (status == -1)
			return (ERR_CD_CHDIR_ERROR);
		return (SUCCESS);
	}
}

static char	*cd_trim_curpath(t_shell **shell, char **curpath)
{
	t_curpath	*curr_dirs_head;
	t_curpath	*curr_dirs_iterator;
	t_curpath	*final_dirs_head;
	t_curpath	*final_dirs_iterator;
	char		*dir;

	curr_dirs_head = cd_split_curpath(*curpath);
	if (!curr_dirs_head)
		return (NULL);
	curr_dirs_iterator = curr_dirs_head;
	final_dirs_head = NULL;
	while (curr_dirs_iterator)
	{
		dir = curr_dirs_iterator->dir;
		if (!dir)
		{
			curpath_free_list(&curr_dirs_head);
			return (NULL);
		}
		if (dir[0] == '.' && dir[1] == '\0')
		{
			curr_dirs_iterator = curr_dirs_iterator->next;
			continue ;
		}
		if (dir[0] == '.' && dir[1] == '.' && dir[2] == '\0')
		{
			final_dirs_iterator = final_dirs_head;
			while (final_dirs_iterator->next != NULL)
				final_dirs_iterator = final_dirs_iterator->next;
			if (final_dirs_iterator->previous)
			{
				dir = final_dirs_iterator->previous->dir;
				if (!dir)
				{
					curpath_free_list(&curr_dirs_head);
					return (NULL);
				}
				else
				{
					final_dirs_iterator = final_dirs_iterator->previous;
					final_dirs_iterator = final_dirs_iterator->previous; // This can be either true or NULL
					final_dirs_iterator = final_dirs_iterator->previous; // This can either be true or segfault
					final_dirs_iterator->next = NULL; // Umm check if this modifies final_dirs_head.
				}
			}
			else
			{
				curpath_free_list(&curr_dirs_head);
				return (ft_strdup("/"));
			}
		}
		dir = ft_strdup(final_dirs_head->dir);
		final_dirs_head = final_dirs_head->next;
		while (final_dirs_head)
		{
			dir = ft_strjoin_fs1(dir, final_dirs_head->dir);
			final_dirs_head = final_dirs_head->next;
		}
		return (dir);
	}
}

t_curpath	*cd_split_curpath(char *curpath)
{
	t_curpath	*head;
	t_curpath	*current;
	char		**directories;
	int			i;

	if (curpath == NULL || *curpath == '\0')
		return (NULL);
	directories = ft_split(curpath, '/');
	if (!directories || !*directories)
		return (NULL);
	if ((*directories[0]) == '\0') //Is this neccessary?
		ft_free_2d((void ***) &directories);
	head = curpath_new_node(directories[0], NULL, NULL);
	if (!head)
		ft_free_2d((void ***) &directories);
	current = head;
	i = 1;
	while (directories[i])
	{
		current->next = curpath_new_node(directories[i], current, NULL);
		if (!current->next)
		{
			ft_free_2d((void ***) &directories);
			curpath_free_list(&head);
		}
		current = current->next;
		i++;
	}
	ft_free_2d((void ***) &directories);
	return (head);
}

// void	execute_cd(t_shell **data, char *directory)
// {
// 	char	*curpath;
// 	t_env	*node;
// 	int		cdpath_keys;
// 	int		i;

// 	if (!directory)
// 	{
// 		node = env_find_node("HOME");
// 		if (!node || !node.key)
// 		{
// 			print_error(ERR_CODE);
// 			return ;
// 		}
// 		else
// 		{
// 			chdir(node.key);
// 			return ;
// 		}
// 	}
// 	else if (directory[0] == '/')
// 		curpath = directory;
// 	else if (directory[0] == '.' || (directory[0] == '.' && directory[1] == '.'))
// 		curpath = directory;
// 	else
// 	{
// 		node = env_find_node("CDPATH");
// 		if (!node || !node.keys)
// 		{
// 			curpath = ft_strjoin_fs1(ft_strdup("./"), directory);
// 			if (!access(curpath, F_OK))
// 				curpath = NULL;
// 			else
// 			{
// 				chdir(curpath);
// 				return ;
// 			}
// 		}
// 		cdpath_keys = node.keycount;
// 		i = 0;
// 		while (cdpath_keys > i)
// 		{
// 			if (!node.keys[i])
// 			{
// 				i++;
// 				continue ;
// 			}
// 			curpath = node.keys[i];
// 			if (curpath[ft_strlen(node.keys[i]) - 1] != '/')
// 				curpath = ft_strjoin_fs1(curpath, "/");
// 			curpath = ft_strjoin_fs1(curpath, directory);
// 			if (access(curpath, F_OK))
// 			{
// 				chdir(curpath);
// 				return ;
// 			}
// 			curpath = NULL;
// 			i++;			
// 		}
// 		curpath = directory;
// 		//Step 7 onwards...
// 	}
	

// }
