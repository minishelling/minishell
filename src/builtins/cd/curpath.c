/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   curpath.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/23 12:54:11 by lprieri       #+#    #+#                 */
/*   Updated: 2024/07/23 13:52:55 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/builtins.h"
#include "../../../include/minishell.h"

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

void	curpath_del_node(t_curpath **node)
{
	if (!*node)
		return ;
	if ((*node)->dir)
		free((*node)->dir);
	free(*node);
	*node = NULL;
	return ;
}

void	curpath_del_list(t_curpath **head) //Maybe reimplement taking a function ptr as arg, a function that handles the freeing of the nodes.
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

t_curpath	*curpath_get_last(t_curpath *head)
{
	t_curpath	*iterator;

	if (!head)
		return (NULL);
	iterator = head;
	while (iterator && iterator->next)
		iterator = iterator->next;
	return (iterator);
}

void	curpath_add_back(t_curpath **head, t_curpath *new)
{
	t_curpath	*iterator;

	if (!new)
		return ;
	if (!*head)
	{
		*head = new;
		(*head)->previous = NULL;
	}
	else
	{
		iterator = curpath_get_last(*head);
		iterator->next = new;
		iterator->next->previous = iterator;
	}
}

void	curpath_del_last(t_curpath **head)
{
	t_curpath	*iterator;

	if (!*head)
		return ;
	iterator = curpath_get_last(*head)->previous;
	curpath_del_node(&iterator->next);
	iterator->next = NULL;
	return ;	
}

char	*curpath_concat(t_curpath *head)
{
	char	*curpath;
	
	if (!head)
		return (NULL);
	curpath = ft_strdup(head->dir);
	if (!curpath)
		return (NULL);
	head = head->next;
	while (head)
	{
		if (head->dir)
		{
			curpath = ft_strjoin_fs1(curpath, head->dir);
			if (!curpath)
				return (NULL);
		}
		head = head->next;	
	}
	return (curpath);
}

int	curpath_check_access(char *curpath)
{
	int	status;

	if (!curpath)
		return (0);
	status = access(curpath, F_OK);
	if (status != 0)
		return (0);
	status = access(curpath, X_OK);
	if (status != 0)
		return (0);
	return (1);
}

