#include "../../../include/minishell.h"

t_ecode	create_and_add_back_curpath_node(t_curpath **head, char *directory)
{
	t_curpath	*new;

	new = curpath_new_node(directory);
	if (!new)
		return (MALLOC_ERROR);
	curpath_add_back(head, new);
	return (SUCCESS);
}

t_curpath	*curpath_new_node(char *dir)
{
	t_curpath	*new_node;

	new_node = (t_curpath *) malloc(sizeof(t_curpath));
	if (!new_node)
		return (NULL);
	new_node->dir = ft_strdup(dir);
	if (!new_node->dir)
	{
		ft_free((void **) &new_node);
		return (NULL);
	}
	new_node->previous = NULL;
	new_node->next = NULL;
	return (new_node);
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
		iterator = get_last_curpath_node(*head);
		iterator->next = new;
		iterator->next->previous = iterator;
	}
}

t_ecode	remove_curpath_node(t_curpath **head, t_curpath **node)
{
	t_curpath	*previous_node;
	// t_curpath	*next_node;

	if (!*node || !*head)
		return (NODE_NOT_FOUND);
	previous_node = (*node)->previous;
	curpath_del_node(node);
	// *node = previous_node;
	return (SUCCESS);
}

void	curpath_del_node(t_curpath **node)
{
	if (!*node)
		return ;
	if (*node && (*node)->dir)
		ft_free((void **) &(*node)->dir);
	if ((*node)->previous)
	{
		// printf("removing previous node in curpath_del_node\n");
		(*node)->previous->next = (*node)->next;
	}
	if ((*node)->next)
	{
		// printf("removing next node in curpath_del_node\n");
		(*node)->next->previous = (*node)->previous;
	}
	free(*node);
	*node = NULL;
	return ;
}

void	curpath_del_node_definitely(t_curpath **node)
{
	if (!*node)
		return ;
	if (*node && (*node)->dir)
		ft_free((void **) &(*node)->dir);
	free(*node);
	*node = NULL;
	return ;
}

void	curpath_del_list(t_curpath **head) //Maybe reimplement taking a function ptr as arg, a function that handles the freeing of the nodes.
{
	t_curpath	*iterator;
	t_curpath	*current;

	if (!*head)
		return ;
	current = *head;
	iterator = *head;
	while (iterator)
	{
		iterator = iterator->next;
		curpath_del_node_definitely(&current);
		current = iterator;
	}
}

t_curpath	*get_last_curpath_node(t_curpath *head)
{
	t_curpath	*iterator;

	if (!head)
		return (NULL);
	iterator = head;
	while (iterator && iterator->next)
		iterator = iterator->next;
	return (iterator);
}

void	curpath_del_last(t_curpath **head)
{
	t_curpath	*iterator;

	if (!*head)
		return ;
	iterator = get_last_curpath_node(*head)->previous;
	if (!iterator)
		return ;
	curpath_del_node_definitely(&iterator->next);
	iterator->next = NULL;
	return ;	
}

int	curpath_check_access(char *curpath)
{
	int	e_status;

	if (!curpath)
		return (1);
	e_status = access(curpath, F_OK);
	if (e_status)
		return (e_status);
	e_status = access(curpath, X_OK);
	if (e_status)
		return (e_status);
	return (0);
}

t_ecode	curpath_check_access_and_chdir(char *curpath)
{
	t_ecode	e_status;

	e_status = curpath_check_access(curpath);
	if (e_status)
		return (ACCESS_ERROR);
	e_status = chdir(curpath);
	if (e_status)
		return (CHDIR_ERROR);
	return (SUCCESS);
}

void	curpath_print(t_curpath *curpath)
{
	int	counter;

	if (!curpath)
	{
		printf("The curpath struct is null, can't print it.\n");
		return ;
	}
	counter = 0;
	while (curpath)
	{
		if (curpath->dir)
			printf("Curpath dir[%d]: %s\n", counter, curpath->dir);
		else
			printf("Curpath dir[%d] is NULL!\n", counter);
		curpath = curpath->next;
		counter++;
	}
	return ;
}
