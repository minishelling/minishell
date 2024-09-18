#include "../../../include/minishell.h"

t_ecode	curpath_trim(char **curpath) //Is this the format of the old version or the new?
{
	t_curpath	*final_dirs;
	char 		**dirs;
	t_ecode		status;

	// printf("In curpath_trim, curpath: %s\n", *curpath);
	final_dirs = NULL;
	dirs = NULL;
	status = init_and_populate_curpath_list(curpath, &dirs, &final_dirs);
	if (status != SUCCESS)
		return (status);
	status = parse_curpath_dirs(&final_dirs);

	curpath_print(final_dirs);

	*curpath = curpath_concat(final_dirs);
	ft_free_2d((void ***) &dirs);
	if (final_dirs)
		curpath_del_list(&final_dirs);
	return (SUCCESS);
}

//Fix the segfault here.
t_ecode	init_and_populate_curpath_list(char **curpath, char ***dirs, t_curpath **final_dirs)
{
	t_ecode	status;
	int		i;
	
	*dirs = ft_split(*curpath, '/');
	if (!dirs)
		return (NULL_ARRAY);
	create_and_add_back_curpath_node(final_dirs, "/");
	i = 0;
	while ((*dirs)[i])
	{
		// printf("In init_and_populate_curpath: dir[%d] is: %s\n", i, (*dirs)[i]);
		status = create_and_add_back_curpath_node(final_dirs, (*dirs)[i]);
		if (status)
			return (status);
		i++;
	}
	// printf("In init_and_populate_curpath:\n");
	// curpath_print(*final_dirs);
	// ft_free((void **) curpath); //Note that there are 2 returns before this, that don't free curpath. Check if it is handled.
	return (SUCCESS);
}


//STOPPED HERE...
t_ecode	parse_curpath_dirs(t_curpath **final_dirs)
{
	t_curpath	*iterator;
	t_curpath	*current;
	t_ecode		status;
	size_t		dir_len;

	status = SUCCESS;
	iterator = *final_dirs;
	while (iterator)
	{
		if (!iterator->dir)
			printf("While parsing curpath, no dir was found in this node.\n");
		dir_len = ft_strlen(iterator->dir);
		if (iterator->dir && !ft_strncmp(iterator->dir, ".", dir_len))
		{
			printf("Removing . from curpath\n");
			current = iterator;
			iterator = iterator->next;
			remove_curpath_node(final_dirs, &current);
			continue ;
		}
		else if (iterator->dir && !ft_strncmp(iterator->dir, "..", dir_len))
		{
			printf("Removing .. from curpath\n");
			if (iterator->previous && !ft_strncmp(iterator->previous->dir, "/", ft_strlen(iterator->previous->dir)))
			{
				current = iterator->previous;
				remove_curpath_node(final_dirs, &current);
				current = iterator;
			}
			else
				current = iterator;
			iterator = iterator->next;
			remove_curpath_node(final_dirs, &current);
			// remove_curpath_node(final_dirs, &current); //Check if the path is actually valid.
			continue ;
		}
		iterator = iterator->next;
	}
	curpath_print(*final_dirs);
	return (SUCCESS);
}

char	*curpath_concat(t_curpath *head)
{
	char	*temp_curpath;
	t_ecode	status;
	
	if (!head || !head->dir)
		return (NULL);
	// printf("head->dir in curpath_concat: %s\n", head->dir);
	temp_curpath = ft_strdup(head->dir);
	if (!temp_curpath)
		return (NULL);
	head = head->next;
	// printf("temp_curpath in curpath_concat: %s\n", temp_curpath);
	while (head && head->dir)
	{
		
		temp_curpath = ft_strjoin_fs1(&temp_curpath, head->dir);
		if (!temp_curpath)
			return (NULL);
		status = append_suffix(&temp_curpath, "/", false);
		if (status)
			return (NULL);
		head = head->next;
		// printf("temp_curpath in curpath_concat: %s\n", temp_curpath);
	}
	// printf("Returning temp_curpath in curpath_concat: %s\n", temp_curpath);
	return (temp_curpath);
}

t_ecode	curpath_prepare(char **curpath, char *directory, char *cwd)
{
	int		status;
	
	status = append_suffix(&cwd, "/", false); //This affects the local cwd variable, right?
	// printf("In curpath_prepare: cwd: %s\n", cwd);
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


t_ecode	remove_previous_dir(t_curpath **final_dirs, char ***dirs, int *i)
{
	char	*temp_curpath;
	t_ecode	status;

	temp_curpath = curpath_concat(*final_dirs);
	if (!temp_curpath)
		return (MALLOC_ERROR);
	printf("temp_curpath in remove_prev_dir: %s\n", temp_curpath);
	status = curpath_check_access(temp_curpath);
	ft_free((void **) &temp_curpath);
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
	
	// curpath = NULL;
	// if (final_dirs && *final_dirs && (*final_dirs)->dir)
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
			status = create_and_add_back_curpath_node(final_dirs, "/");
			if (status)
				return (status);
		}
		status = create_and_add_back_curpath_node(final_dirs, (*dirs)[*i]);
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
// 				status = create_and_add_back_curpath_node(&final_dirs, &dirs, "/");
// 				if (status)
// 					return (status);
// 			}
// 			status = create_and_add_back_curpath_node(&final_dirs, &dirs, dirs[i]);
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