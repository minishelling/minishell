#include "../../../include/builtins.h"
#include "../../../include/minishell.h"

typedef enum flags
{
	NULL_FLAG = 0,
	STATUS_FLAG
}	t_flags;

t_ecode	chdir_cwd(char *directory, int *null_flag)
{
	char	*curpath;

	*null_flag = 1;
	curpath = ft_strjoin("./", directory);
	if (!curpath)
		return (ft_free((void **) &curpath), MALLOC_ERROR);
	if (curpath_check_access(curpath))
		return (ft_free((void **) &curpath), PROCEED);
	if (chdir(curpath))
		return (ft_free((void **) &curpath), PROCEED);
	else
		return (ft_free((void **) &curpath), SUCCESS);
}

t_ecode chdir_cdpath_value(char **curpath, char *directory)
{
	t_ecode	status;

	status = append_suffix(curpath, "/", false);
	if (status)
		return (MALLOC_ERROR);
	*curpath = ft_strjoin_fs1(curpath, directory);
	if (!*curpath)
		return (MALLOC_ERROR);
	status = curpath_check_access(*curpath);
	if (status)
		return (ft_free((void **) curpath), PROCEED);
	status = chdir(*curpath);
	if (status)
		return (ft_free((void **) curpath), PROCEED);
	else
		return (ft_free((void **) curpath), SUCCESS);
}

t_ecode	loop_cdpath_values(char ***values, char *directory)
{
	char	*curpath;
	int		i;
	int		flag[2];

	i = 0;
	flag[NULL_FLAG] = 0;
	flag[STATUS_FLAG] = 0;
	while ((*values)[i])
	{
		curpath = ft_strdup((*values)[i]);
		if (!curpath && flag[NULL_FLAG])
		{
			flag[STATUS_FLAG] = chdir_cwd(directory, &flag[NULL_FLAG]);
			if (flag[STATUS_FLAG] == PROCEED)
			{
				i++;
				continue ;
			}
			else
				return (ft_free_2d((void ***) values), flag[STATUS_FLAG]);
		}
		else if (!curpath && flag[NULL_FLAG])
		{
			i++;
			continue;
		}
		flag[STATUS_FLAG] = chdir_cdpath_value(&curpath, directory);
		if (flag[STATUS_FLAG] == PROCEED)
		{
			ft_free((void **) &curpath);
			i++;
			continue ;
		}
		else
			return (ft_free((void **) &curpath),
				ft_free((void ***) values), flag[STATUS_FLAG]);
	}
	return (ft_free((void ***) values), PROCEED);
}
