/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cd.c                                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/31 12:47:36 by lprieri       #+#    #+#                 */
/*   Updated: 2024/07/31 12:47:36 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/builtins.h"
#include "../../../include/minishell.h"

typedef enum
{
	CURPATH,
	CURWD,
	OLDWD,
	TEMP
}	t_cdvar; //For storing strings. I don't have enough space to declare many, but I can like this.

t_cd_ecode	cd_chdir_home(t_env *env)
{
	t_env	*home_node;
	int		e_status;

	home_node = env_find_node(env, "HOME");
	if (!home_node || !home_node->value)
		return (CD_NO_HOME);
	else
	{
		e_status = chdir(home_node->value);
		if (e_status)
			return (CD_CHDIR_ERROR); //Errno is also set.
		return (CD_CHDIR_SUCCESS);
	}
}

t_cd_ecode cd_chdir_cdpath(t_env *env, char *directory)
{
	t_env		*cdpath_node;
	size_t		i;
	size_t		values_count; //Not enough space for this var.
	char		*curpath;
	char		**values;
	int			null_flag;
	t_cd_ecode	e_status;

	cdpath_node = env_find_node(env, "CDPATH");
	if (cdpath_node == NULL || cdpath_node->value == NULL)
		return (CD_PROCEED);
	values_count = env_count_values(env, "CDPATH");
	values = ft_split(cdpath_node->value, ':');
	if (!values)
		return (CD_MALLOC_ERROR);
	i = 0;
	while (i < values_count)
	{
		curpath = ft_strdup(values[i]);
		if (!curpath && !null_flag)
		{
			null_flag = 1;
			curpath = ft_strjoin("./", directory);
			e_status = curpath_check_access(curpath);
			if (e_status)
			{
				(ft_free((void **) &curpath), ft_free_2d((void ***) &values));
				i++;
				continue ;
			}
			e_status = chdir(curpath);
			(ft_free((void **) &curpath), ft_free_2d((void ***) &values));
			if (e_status)
			{
				i++;
				continue ;
			}
			else
				return (CD_CHDIR_SUCCESS);
		}
		else if (!curpath && null_flag)
		{
			i++;
			continue ;
		}
		if (curpath[ft_strlen(curpath) - 1] != '/')
		{
			curpath = ft_strjoin_fs1(&curpath, "/");
			if (!curpath)
				return (CD_MALLOC_ERROR);
		}
		curpath = ft_strjoin_fs1(&curpath, directory);
		if (!curpath)
			return (CD_MALLOC_ERROR);
		e_status = curpath_check_access(curpath);
		if (e_status)
		{
			(ft_free((void **) &curpath), ft_free_2d((void ***) &values));
			i++;
			continue ;
		}
		e_status = chdir(curpath);
		(ft_free((void **) &curpath), ft_free_2d((void ***) &values));
		if (e_status)
		{
			i++;
			continue ;
		}
		else
			return (CD_SUCCESS);
	}
	return (CD_PROCEED);
}

t_cd_ecode	execute_cd(t_env *env, char *directory)
{
	t_cd_ecode	e_status;
	char		*curpath;
	char		*cwd;
	// char		*oldwd;

	cwd = getcwd(NULL, PATH_MAX);
	if (!cwd)
		return (CD_NO_CWD); //ERRNO IS SET
	cwd = ft_strjoin_fs1(&cwd, "/");
	if (!cwd)
		return (CD_MALLOC_ERROR);
	if (!directory)
	{
		e_status = cd_chdir_home(env);
		if (e_status != CD_CHDIR_SUCCESS)
			return (CD_NO_HOME);
		else
			return (CD_CHDIR_SUCCESS);
	}
	else if (directory[0] != '/' && directory[0] != '.'
			&& (directory[0] != '.' && directory[1] != '.'))
	{
		e_status = cd_chdir_cdpath(env, directory);
		if (e_status == CD_SUCCESS)
			return (CD_SUCCESS); //Print the CWD here!
		if (e_status != CD_PROCEED)
			return (CD_CDPATH_ERROR);
	}
	curpath = ft_strdup(directory);
	if (curpath[ft_strlen(curpath) - 1] != '/'
		&& curpath[0] != '/')
	{
		curpath = ft_strjoin_fs2(cwd, &curpath);
		if (!curpath)
			return (CD_MALLOC_ERROR);
	}
	curpath = cd_trim_curpath(&curpath);
	if (!curpath)
		return (CD_MALLOC_ERROR);
	e_status = curpath_check_access(curpath);
	if (e_status)
		return (CD_NO_ACCESS);
	e_status = chdir(curpath);
	if (e_status)
		return (CD_CHDIR_ERROR); //Errno is set
	return (CD_CHDIR_SUCCESS);
}

char	*cd_trim_curpath(char **curpath)
{
	char		**dirs;
	t_curpath	*final_dirs;
	int			i;
	int			status;

	dirs = NULL;
	final_dirs = NULL;
	dirs = ft_split(*curpath, '/');
	if (!dirs)
		return (NULL);
	if (*curpath[0] == '/')
	{
		status = curpath_create_and_add_back(&final_dirs, &dirs, "/");
		if (status)
			return (NULL); //PRINT APPROPRIATE ERROR MESSAGE
	}
	ft_free((void **) curpath);
	i = 0;
	while (dirs[i])
	{
		if (dirs[i][0] == '.' && dirs[i][1] == '\0')
		{
			i++;
			continue ;
		}
		else if (dirs[i][0] == '.' && dirs[i][1] == '.' && dirs[i][2] == '\0')
		{
			*curpath = curpath_concat(final_dirs);
			status = curpath_check_access(*curpath);
			ft_free((void **) curpath);
			if (status)
			{
				ft_free_2d((void ***) &dirs);
				curpath_del_list(&final_dirs);
				return (NULL); //PRINT APPROPRIATE ERROR MESSAGE
			}
			curpath_del_last(&final_dirs);
			i++;
			continue ;
		}
		*curpath = curpath_concat(final_dirs);
		status = curpath_check_access(*curpath);
		ft_free((void **) curpath);
		if (status)
		{
			ft_free_2d((void ***) &dirs);
			if (final_dirs)
				curpath_del_list(&final_dirs);
			return (NULL); //PRINT APPROPRIATE ERROR MESSAGE
		}
		else
		{
			if (final_dirs == NULL)
			{
				status = curpath_create_and_add_back(&final_dirs, &dirs, "/");
				if (status)
					return (NULL); //PRINT APPROPRIATE ERROR MESSAGE
			}
			status = curpath_create_and_add_back(&final_dirs, &dirs, dirs[i]);
			if (status)
				return (NULL); //PRINT APPROPRIATE ERROR MESSAGE
				
		}
		i++;
	}
	*curpath = curpath_concat(final_dirs);
	ft_free_2d((void ***) &dirs);
	if (final_dirs)
		curpath_del_list(&final_dirs);
	return (*curpath);
}

//Old EXECUTE_CD Function that served as prototype
// t_ecode	execute_cd(t_env *env, char *directory)
// {
// 	char	*curpath;
// 	char	*cwd;
// 	void	*temp;
// 	int		i;
// 	int		no_cdpath_flag;

// 	if (!directory) //First step
// 	{
// 		temp = env_find_node("HOME");
// 		if (!temp || !temp.key)
// 			return (ERR_CD_NO_HOME);
// 		else //Second step
// 		{
// 			if (chdir(temp.key) == -1)
// 				return (ERR_CD_CHDIR_ERROR); //Errno is set.
// 		}	
// 	}
// 	else if (directory[0] == '/' || directory[0] == '.'
// 		|| (directory[0] == '.' && directory[1] == '.')) //Steps 3 and 4
// 		curpath = directory;
// 	else //Step 5 - Which was wrong, because it has to set and then proceed to step 6 outside of the else statement.
// 	{
// 		temp = (t_env *)env_find_node("CDPATH"); //Look for CDPATH in env. Retrieve the node. - Testing the use of (void *).
// 		i = 0;
// 		while (i < env_count_values(env, "CDPATH")) //A value can be an empty string. 
// 		{
// 			curpath = ft_strdup(temp.values[i]); //Dups so I can use ft_strjoin_fs1 or simply ft_free on curpath. - Protect malloc.
// 			if (curpath == NULL && no_cdpath_flag == 0) //If the string is empty or is NULL (strdup takes care of handling empty strings, converting them to NULL),
// 			{//Then execute step 4
// 				curpath = ft_strjoin("./", directory); //Append current directory and the directory provided.
// 				if (access(curpath, F_OK) == -1) //Why would I replace access with a dir function?
// 				{
// 					ft_free(&curpath);
// 					i++;
// 					continue ;
// 				}
// 				if (chdir(curpath) == -1)
// 				{
// 					ft_free(&curpath); //Repeated functionality... How can I optimize this?
// 					i++;
// 					continue ;
// 				}
// 				no_cdpath_flag = 1; //Flags this occurrence so it doesn't do it again.
// 			}
// 			else if (curpath == NULL && no_cdpath_flag == 1) //New addition that I forgot in the initial implementation.
// 			{
// 				i++;
// 				continue ;
// 			}
// 			if (curpath[len -1] != '/') //Continues execution when the node found in CDPATH is true.
// 				curpath = ft_strjoin_fs1(curpath, "/"); //Appends a slash if the path in CDPATH didn't end with one. - Protect the malloc.
// 			curpath = ft_strjoin_fs1(curpath, directory); //Appends the path in CDPATH with the directory.
// 			if (access(curpath, F_OK) == -1) //Checks if it is a valid dir.
// 			{
// 				ft_free(&curpath);
// 				i++;
// 				continue ;
// 			}
// 			if (chdir(curpath) == -1) //Checks if changing to that dir fails.
// 			{
// 				ft_free(&curpath);
// 				i++;
// 				continue ;
// 			}
// 			else //Forgot to add the action for when it succeeds. So adding it now:
// 			{
// 				ft_free(&curpath);
// 				//Update pwd and oldpwd.
// 				return (SUCCESS);
// 			}
// 			i++;
// 		}
// 	}
// 	curpath = directory; //Step 6
// 	if (shell->pwd[len - 1] != '/') //Here I can use either PWD from the shell struct (not from the env), or the getcwd command.
// 		shell->pwd = ft_strjoin_fs1(shell->pwd, "/"); //Protect mallocs.

// 	//Shouldn't I be concatenating PWD with curpath here?

// 	curpath = cd_trim_curpath(shell, &curpath); //Return a trimmed path that is ready to be used.
// 	// Check if (ft_strlen(curpath) + 1 > PATH_MAX)
// 	status = chdir(curpath);
// 	free(curpath); //The end of the journey.
// 	if (status == -1) //Status could be called ret, becasue after all it has to return a value.
// 		return (ERR_CD_CHDIR_ERROR);
// 	return (SUCCESS);
// }



//Old function that I'm looking forward to replace.
// static char	*cd_trim_curpath(t_shell **shell, char **curpath)
// {
// 	t_curpath	*curr_dirs_head;
// 	t_curpath	*curr_dirs_iterator;
// 	t_curpath	*final_dirs_head;
// 	t_curpath	*final_dirs_iterator;
// 	char		*dir;

// 	curr_dirs_head = cd_split_curpath(*curpath);// Create array to store each directory separately, without including slashes. - Ensure we are not passing the root directory.
// 	if (!curr_dirs_head)
// 		return (NULL);
// 	curr_dirs_iterator = curr_dirs_head;
// 	final_dirs_head = NULL; //Initialize to null. When appropriate it will add the valid nodes inside the loop.
// 	while (curr_dirs_iterator) //Iterate through the list of directories using a helping iterating variable (keeping the head var to free when neccessary).
// 	{
// 		dir = curr_dirs_iterator->dir; //Assign the dir in curr_dirs to a shorter named var. -- Keep in mind that the first node is okay, but for the rest it needs to keep concatenating them. Maybe add a helper funct.
// 		if (!dir) //If one of the dirs is NULL then the whole path is invalid.
// 		{
// 			curpath_free_list(&curr_dirs_head);
// 			return (NULL);
// 		}
// 		if (dir[0] == '.' && dir[1] == '\0') //If the dir is a single dot, then we just skip this directory. It is not added to final_dirs.
// 		{
// 			curr_dirs_iterator = curr_dirs_iterator->next;
// 			continue ;
// 		}
// 		else if (dir[0] == '.' && dir[1] == '.' && dir[2] == '\0') //If the dir is a double dot...
// 		{
// 			final_dirs_iterator = final_dirs_head;
// 			while (final_dirs_iterator != NULL && final_dirs_iterator->next != NULL)
// 				final_dirs_iterator = final_dirs_iterator->next;
// 			if (final_dirs_iterator != NULL && final_dirs_iterator->previous)
// 			{
// 				dir = final_dirs_iterator->previous->dir;
// 				if (!dir)
// 				{
// 					curpath_free_list(&curr_dirs_head);
// 					return (NULL);
// 				}
// 				else
// 				{
// 					final_dirs_iterator = final_dirs_iterator->previous;
// 					final_dirs_iterator = final_dirs_iterator->previous; // This can be either true or NULL
// 					final_dirs_iterator = final_dirs_iterator->previous; // This can either be true or segfault
// 					final_dirs_iterator->next = NULL; // Umm, check if this actually modifies final_dirs_head.
// 				}
// 			}
// 			else
// 			{
// 				curpath_free_list(&curr_dirs_head);
// 				return (ft_strdup("/"));
// 			}
// 		}
// 		else
// 		{
// 			//Ummm, what did I do here? It needs to keep iterating, right?
// 		}
// 		dir = ft_strdup(final_dirs_head->dir);
// 		final_dirs_head = final_dirs_head->next;
// 		while (final_dirs_head)
// 		{
// 			dir = ft_strjoin_fs1(dir, final_dirs_head->dir);
// 			final_dirs_head = final_dirs_head->next;
// 		}
// 		return (dir);
// 	}
// }


//It's not going to be needed since I'll use ft_split to create an array.
// t_curpath	*cd_split_curpath(char *curpath)
// {
// 	t_curpath	*head;
// 	t_curpath	*current;
// 	char		**directories;
// 	int			i;

// 	if (curpath == NULL || *curpath == '\0')
// 		return (NULL);
// 	directories = ft_split(curpath, '/');
// 	if (!directories || !*directories)
// 		return (NULL);
// 	if ((*directories[0]) == '\0') //Is this neccessary?
// 		ft_free_2d((void ***) &directories);
// 	head = curpath_new_node(directories[0], NULL, NULL);
// 	if (!head)
// 		ft_free_2d((void ***) &directories);
// 	current = head;
// 	i = 1;
// 	while (directories[i])
// 	{
// 		current->next = curpath_new_node(directories[i], current, NULL);
// 		if (!current->next)
// 		{
// 			ft_free_2d((void ***) &directories);
// 			curpath_free_list(&head);
// 		}
// 		current = current->next;
// 		i++;
// 	}
// 	ft_free_2d((void ***) &directories);
// 	return (head);
// }
