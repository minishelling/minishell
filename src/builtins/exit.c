#include "../../include/minishell.h"
 
static t_ecode handle_argc_2(char **cmd_args)
{
	if (!ft_strncmp(cmd_args[1], "--", 3))
	{
		ft_putstr_fd("exit\n", 2);
		exit(SUCCESS);
	}
	else if (ft_is_natural(cmd_args[1]))
	{
		ft_putstr_fd("exit\n", 2);
		exit(ft_atoi(cmd_args[1]));
	}
	else
	{
		handle_builtin_err(cmd_args[0], cmd_args[1], "numeric argument required");
		exit(2);
	}
}

static t_ecode handle_argc_gt_2(char **cmd_args, size_t argc)
{
	if (!ft_strncmp(cmd_args[1], "--", 3))
	{
		if (ft_is_natural(cmd_args[2]) && argc == 3)
		{
			ft_putstr_fd("exit\n", 2);
			exit(ft_atoi(cmd_args[2]));
		}
		else if (!ft_is_natural(cmd_args[2]))
		{
			handle_builtin_err(cmd_args[0], cmd_args[1], "numeric argument required");
			exit(2);
		}
		else
		{
			ft_putstr_fd("exit\n", 2);
			handle_builtin_err(cmd_args[0], cmd_args[1], "too many arguments");
			return (FAILURE);
		}
	}
	else if (!ft_is_natural(cmd_args[1]))
	{
		handle_builtin_err(cmd_args[0], cmd_args[1], "numeric argument required");
		exit(2); //Return 2
	}
	else
	{
		ft_putstr_fd("exit\n", 2);
		handle_builtin_err(cmd_args[0], cmd_args[1], "too many arguments");
		return (FAILURE); //Return ERROR == -1, so the main prints exit and whatever...
	}
}


t_ecode	exit_builtin(t_shell *shell, char **cmd_args)
{
	size_t	arg_count;
	int		exit_code;

	(void) shell;
	arg_count = ft_str_count(cmd_args);
	if (arg_count == 2)
	{
		exit_code = handle_argc_2(cmd_args);
	}
	else if (arg_count > 2)
	{
		exit_code = handle_argc_gt_2(cmd_args, arg_count);
	}
	else
	{
		ft_putstr_fd("exit\n", 2);
		exit(SUCCESS);
	}
	return (exit_code);
}
