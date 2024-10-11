#include "../../include/minishell.h"

void handle_builtin_err(char *cmd_name, char *arg, char *err_msg)
{
	char *full_msg;
	size_t total_len;
	const char *prompt = ERR_PROMPT;
	const char *colon = ": ";
	const char *newline = "\n";

	// Calculate total length: prompt + command + colon + arg + colon + space + error message + newline
	total_len = ft_strlen(prompt) + ft_strlen(cmd_name) +
		ft_strlen(colon) + ft_strlen(err_msg) + ft_strlen(newline);
	full_msg = (char *)malloc(total_len + 1);
	if (!full_msg)
		return; // handle malloc failure, I am not sure how - there is a problem already
	ft_strlcpy(full_msg, prompt, ft_strlen(prompt) + 1);
	ft_strlcat(full_msg, cmd_name, total_len + 1);
	ft_strlcat(full_msg, colon, total_len + 1);
	if (arg)
	{
		total_len += ft_strlen(arg) + ft_strlen(colon);
		ft_strlcat(full_msg, arg, total_len + 1);
		ft_strlcat(full_msg, colon, total_len + 1);
	}
	ft_strlcat(full_msg, err_msg, total_len + 1);
	ft_strlcat(full_msg, newline, total_len + 1);
	write(2, full_msg, total_len);
	free(full_msg);
}

// void handle_builtin_err(char *cmd_name, char *err_msg)
// {
// 	char *full_msg;
// 	size_t total_len;
// 	const char *prompt = ERR_PROMPT;
// 	const char *colon = ": ";
// 	const char *newline = "\n";

// 	// Calculate total length: prompt + command + colon + space + error message + newline
// 	total_len = ft_strlen(prompt) + ft_strlen(cmd_name) + ft_strlen(colon) + 
// 				ft_strlen(err_msg) + ft_strlen(newline);
// 	full_msg = (char *)malloc(total_len + 1);
// 	if (!full_msg)
// 		return; // handle malloc failure, I am not sure how - there is a problem already
// 	ft_strlcpy(full_msg, prompt, ft_strlen(prompt) + 1);
// 	ft_strlcat(full_msg, cmd_name, total_len + 1);
// 	ft_strlcat(full_msg, colon, total_len + 1);
// 	ft_strlcat(full_msg, err_msg, total_len + 1);
// 	ft_strlcat(full_msg, newline, total_len + 1);
// 	write(2, full_msg, total_len);
// 	free(full_msg);
// }
