#include "../../include/minishell.h"

#define COLON ": "
#define _NEWLINE "\n"

void	handle_builtin_err(char *cmd_name, char *arg, char *err_msg)
{
	char	*full_msg;
	size_t	total_len;

	total_len = ft_strlen(ERR_PROMPT) + ft_strlen(cmd_name)
		+ ft_strlen(COLON) + ft_strlen(err_msg) + ft_strlen(_NEWLINE);
	if (arg)
		total_len += ft_strlen(arg) + ft_strlen(COLON);
	full_msg = (char *)malloc(total_len + 1);
	if (!full_msg)
	{
		perror("handle_builtin_err");
		return ;
	}
	ft_strlcpy(full_msg, ERR_PROMPT, ft_strlen(ERR_PROMPT) + 1);
	ft_strlcat(full_msg, cmd_name, total_len + 1);
	ft_strlcat(full_msg, COLON, total_len + 1);
	if (arg)
	{
		ft_strlcat(full_msg, arg, total_len + 1);
		ft_strlcat(full_msg, COLON, total_len + 1);
	}
	ft_strlcat(full_msg, err_msg, total_len + 1);
	ft_strlcat(full_msg, _NEWLINE, total_len + 1);
	write(2, full_msg, total_len);
	free(full_msg);
}
