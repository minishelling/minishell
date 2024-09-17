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

t_ecode	curpath_create_and_add_back(t_curpath **head, char ***dirs, char *dir)
{
	t_curpath	*new;

	new = curpath_new_node(dir, NULL, NULL);
	if (!new)
	{
		ft_free_2d((void ***) dirs);
		curpath_del_list(head);
		return (NODE_NOT_FOUND);
	}
	curpath_add_back(head, new);
	return (SUCCESS);
}

void	curpath_del_last(t_curpath **head)
{
	t_curpath	*iterator;

	if (!*head)
		return ;
	iterator = curpath_get_last(*head)->previous;
	if (!iterator)
		return ;
	curpath_del_node(&iterator->next);
	iterator->next = NULL;
	return ;	
}

char	*curpath_concat(t_curpath *head)
{
	char	*curpath;
	
	if (!head)
		return (NULL);
	if (head->dir && head->dir[0] != '/')
	{
		curpath = ft_strdup("/");
		if (!curpath)
			return (NULL);
	}
	else
		curpath = ft_strdup("");
	while (head)
	{
		if (head->dir)
		{
			curpath = ft_strjoin_fs1(&curpath, head->dir);
			if (!curpath)
				return (NULL);
			if (curpath[ft_strlen(curpath) - 1] != '/')
			{
				curpath = ft_strjoin_fs1(&curpath, "/");
				if (!curpath)
					return (NULL);
			}
		}
		head = head->next;	
	}
	return (curpath);
}

void	curpath_print(t_curpath *head)
{
	int	i;

	if (!head)
	{
		printf("Head is null in curpath_print\n");
		return ;
	}
	i = 0;
	while (head)
	{
		if (head->dir)
			printf("Curpath dir[%i]: %s\n", i, head->dir);
		i++;
		head = head->next;
	}
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

int	curpath_check_access_and_chdir(char *curpath)
{
	int	e_status;

	e_status = curpath_check_access(curpath);
	if (e_status)
		return (ACCESS_ERROR);
	e_status = chdir(curpath);
	if (e_status)
		return (CHDIR_ERROR);
	return (SUCCESS);
}
