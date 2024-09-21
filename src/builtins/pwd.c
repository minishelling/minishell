#include "../../include/minishell.h"

t_ecode	pwd_builtin(t_shell *shell, char **cmd_args)
{
	char	*pwd;
	size_t	option_len;

	(void) shell;
	pwd = getcwd(NULL, PATH_MAX);
	if (!pwd)
		return (CWD_ERROR);
	if (cmd_args && cmd_args[1] && cmd_args[1][0] == '-')
	{
		option_len = ft_strlen(cmd_args[1]);
		if (!ft_strncmp(cmd_args[1], "-", option_len) || !ft_strncmp(cmd_args[1], "--", option_len));
		else
			return (INVALID_OPTION);
	}
	ft_putstr_fd(pwd, 1);
	ft_putchar_fd('\n', 1);
	ft_free((void **) &pwd);
	return (SUCCESS);
}
