#include "../../include/minishell.h"

char *get_err_msg(int e)
{
	char *error_messages[] = { 
		"OK",
		"syntax error near unexpected token `newline`\n",
		"Syntax error: unclosed quotes\n",
		"syntax error near unexpected token `(`\n",
		"syntax error near unexpected token `)`\n",
		"Syntax error near unexpected token `|` \n",
		"Syntax error near unexpected token `||`\n",
		"Syntax error near unexpected token `&&`\n",
		"Syntax error near unexpected token `<` or `>`\n",
		"Syntax error\n",
		"Error: unable to allocate dynamic memory\n",
		"SIGINT_HDOC"
	};
	return (error_messages[e]);
}

void free_tree(t_tree **node)
{
	if (*node == NULL)
		return;
	free_tree(&(*node)->left);
	free_tree(&(*node)->right);
	if ((*node)->cmd)
		free_cmd(&(*node)->cmd);
	free(*node);
	*node = NULL;
}

void clean_nicely(t_shell *shell, void* param)
{
	(void)param;
	if (shell->input)
	{
		free(shell->input);
		shell->input = NULL;
	}
	if (shell->token)
		free_token_list(&shell->token);
	if (shell->tree)
		free_tree(&shell->tree);

}

void handle_parsing_err(t_shell *shell, int err_no, void *param)
{
	char *err_msg;
	char *err_prompt;
	char *full_msg;
	size_t full_msg_len;

	(void)shell;
	(void)param;

	if (err_no != SIGINT_HDOC)
	{
		err_prompt = ERR_PROMPT;
		err_msg = get_err_msg(err_no);
		full_msg = ft_strjoin(err_prompt, err_msg);
		if (!full_msg)
			exit(EXIT_FAILURE); //??
		full_msg_len = ft_strlen(full_msg);
		write(2, full_msg, full_msg_len);
		free(full_msg);
	}
	// clean_nicely(shell, param); ??
}

void handle_cmd_err(t_cmd *cmd, char *err_msg)
{
	char *full_msg;
	size_t cmd_len;
	size_t err_len;
	size_t total_len;
	const char *colon = ": ";
	const char *newline = "\n";

	cmd_len = ft_strlen(cmd->args[0]);
	err_len = ft_strlen(err_msg);
	// Calculate total length: command + colon + space + error message + newline
	total_len = cmd_len + ft_strlen(colon) + err_len + ft_strlen(newline);
	full_msg = (char *)malloc(total_len + 1);
	if (!full_msg)
		return; // handle malloc failure, I am not sure how - there is a problem already
	ft_strlcpy(full_msg, cmd->args[0], cmd_len + 1);
	ft_strlcat(full_msg, colon, total_len + 1);
	ft_strlcat(full_msg, err_msg, total_len + 1);
	ft_strlcat(full_msg, newline, total_len + 1);
	write(2, full_msg, total_len);
	free(full_msg);
}


void handle_perror(char *str)
{
	const char *err_prompt;
	char *full_err_msg;
	size_t cmd_len, prompt_len, total_len;

	if (!str)
		return;
	err_prompt = ERR_PROMPT;
	cmd_len = ft_strlen(str);
	prompt_len = ft_strlen(err_prompt);
	total_len = cmd_len + prompt_len;
	full_err_msg = (char *)malloc(total_len + 1);
	if (!full_err_msg)
		return; // ???
	ft_strlcpy(full_err_msg, err_prompt, total_len + 1);
	ft_strlcat(full_err_msg, str, total_len + 1);
	perror(full_err_msg);
	free(full_err_msg);
}

