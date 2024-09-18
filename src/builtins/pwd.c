#include "../../include/minishell.h"

int	pwd_builtin(char **args)
{
	char	*pwd;
	size_t	option_len;

	pwd = getcwd(NULL, PATH_MAX);
	if (!pwd)
		return (CWD_ERROR);
	if (args && args[1] && args[1][0] == '-')
	{
		option_len = ft_strlen(args[1]);
		if (!ft_strncmp(args[1], "-", option_len) || !ft_strncmp(args[1], "--", option_len));
		else
			return (INVALID_OPTION);
	}
	ft_putstr_fd(pwd, 1);
	ft_putchar_fd('\n', 1);
	ft_free((void **) &pwd);
	return (SUCCESS);
}
