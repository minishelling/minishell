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
		return (ENV_ERROR);
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

t_ecode	curpath_prepare(char **curpath, char *directory, char *cwd)
{
	int		status;
	
	status = append_suffix(&cwd, "/", false); //This affects the local cwd variable, right?
	if (status == MALLOC_ERROR)
		return (MALLOC_ERROR);
	if (directory[0] != '/')
		*curpath = ft_strjoin(cwd, directory);
	else
		*curpath = ft_strdup(directory);
	if (!curpath)
		return (MALLOC_ERROR);
	else
		return (SUCCESS);
}

t_ecode	curpath_trim(char *directory, char **curpath) //Is this the format of the old version or the new?
{
	t_curpath	*final_dirs;
	char 		**dirs;
	t_ecode		status;

	(void) directory; //Unused variable??
	status = init_curpath_dirs(curpath, &dirs, &final_dirs);
	if (status != SUCCESS)
		return (status);
	status = parse_curpath_dirs(&final_dirs, &dirs);
	*curpath = curpath_concat(final_dirs);
	ft_free_2d((void ***) &dirs);
	if (final_dirs)
		curpath_del_list(&final_dirs);
	return (SUCCESS);
}

t_ecode	init_curpath_dirs(char **curpath, char ***dirs, t_curpath **final_dirs)
{
	t_ecode	status;
	
	*dirs = ft_split(*curpath, '/');
	if (!dirs)
		return (NULL_ARRAY);
	if (*curpath[0] == '/')
	{
		status = curpath_create_and_add_back(final_dirs, dirs, "/");
		if (status)
			return (status);
	}
	ft_free((void **) curpath); //Note that there are 2 returns before this, that don't free curpath. Check if it is handled.
	return (SUCCESS);
}

t_ecode	parse_curpath_dirs(t_curpath **final_dirs, char ***dirs)
{
	int		i;
	t_ecode	status;

	i = 0;
	while ((*dirs)[i])
	{
		if ((*dirs)[i][0] == '.' && (*dirs)[i][1] == '\0')
		{
			i++;
			continue ;
		}
		else if ((*dirs)[i][0] == '.' && (*dirs)[i][1] == '.' && (*dirs)[i][2] == '\0')
		{
			status = remove_previous_dir(final_dirs, dirs, &i);
			if (status)
				return (status);
			continue ;
		}
		status = check_access_and_add_back(final_dirs, dirs, &i);
	}
	return (status);
}

t_ecode	remove_previous_dir(t_curpath **final_dirs, char ***dirs, int *i)
{
	char	*curpath;
	t_ecode	status;

	curpath = curpath_concat(*final_dirs);
	status = curpath_check_access(curpath);
	ft_free((void **) &curpath);
	if (status)
	{
		ft_free_2d((void ***) dirs);
		if (*final_dirs)
			curpath_del_list(final_dirs);
		return (status);
	}
	curpath_del_last(final_dirs);
	*i += 1;
	return (SUCCESS);
}

t_ecode	check_access_and_add_back(t_curpath **final_dirs, char ***dirs, int *i)
{
	char	*curpath;
	t_ecode	status;
	
	curpath = curpath_concat(*final_dirs);
	status = curpath_check_access(curpath);
	ft_free((void **) &curpath);
	if (status)
	{
		ft_free_2d((void ***) dirs);
		if (*final_dirs)
			curpath_del_list(final_dirs);
		return (status);
	}
	else
	{
		if (!*final_dirs)
		{
			status = curpath_create_and_add_back(final_dirs, dirs, "/");
			if (status)
				return (status);
		}
		status = curpath_create_and_add_back(final_dirs, dirs, (*dirs)[*i]);
		if (status)
			return (status);
	}
	*i += 1;
	return (SUCCESS);
}

/** NOTE TO SELF
 * I need to take a look at all the functions and distinguish or discriminate between
 * those that take arguments for freeing everything (basically error handling within the function)
 * and those that don't.
 * In the end I have to decide for one way of writing functions or the other.
*/




// t_ecode	curpath_trim(char **curpath)
// {
// 	t_curpath	*final_dirs;
// 	char 		**dirs;
// 	int			i;
// 	int			status;

// 	status = init_curpath_dirs(curpath, &dirs, &final_dirs);
// 	if (status != SUCCESS)
// 		return (status);
// 	i = 0;
// 	while (dirs[i])
// 	{
// 		if (dirs[i][0] == '.' && dirs[i][1] == '/0')
// 		{
// 			i++;
// 			continue ;
// 		}
// 		else if (dirs[i][0] == '.' && dirs[i][1] == '.' && dirs[i][2] == '/0')
// 		{
// 			*curpath = curpath_concat(final_dirs);
// 			status = curpath_check_access(*curpath);
// 			ft_free((void **) curpath);
// 			if (status)
// 			{
// 				ft_free_2d((void ***) &dirs);
// 				if (final_dirs)
// 					curpath_del_list(&final_dirs);
// 				return (status);
// 			}
// 			curpath_del_last(&final_dirs);
// 			i++;
// 			continue ;
// 		}
// 		*curpath = curpath_concat(final_dirs);
// 		status = curpath_check_access(*curpath);
// 		ft_free((void **) curpath);
// 		if (status)
// 		{
// 			ft_free_2d((void ***) &dirs);
// 			if (final_dirs)
// 				curpath_del_list(&final_dirs);
// 			return (status);
// 		}
// 		else
// 		{
// 			if (!final_dirs)
// 			{
// 				status = curpath_create_and_add_back(&final_dirs, &dirs, "/");
// 				if (status)
// 					return (status);
// 			}
// 			status = curpath_create_and_add_back(&final_dirs, &dirs, dirs[i]);
// 			if (status)
// 				return (status);
// 		}
// 		i++;
// 	}
// 	*curpath = curpath_concat(final_dirs);
// 	ft_free_2d((void ***) &dirs);
// 	if (final_dirs)
// 		curpath_del_list(&final_dirs);
// 	return (*curpath);
// }