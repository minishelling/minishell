#include "../../include/minishell.h"

size_t	get_arg_num(t_token *start_token, t_token *end_token)
{
	size_t	arg_count;
	t_token	*cur_token;

	arg_count = 0;
	cur_token = start_token;
	while (cur_token)
	{
		if (cur_token->id == WORD || cur_token->id == SQUOTE || \
			cur_token->id == DQUOTE || \
			(cur_token->id == ENV_VAR && *(cur_token->str) != '\0'))
			arg_count++;
		if (cur_token->id == LT || cur_token->id == GT)
			arg_count--;
		if (cur_token == end_token)
			break ;
		cur_token = cur_token->next;
	}
	return (arg_count);
}

int	build_command_from_token(t_cmd *cmd, t_token *token)
{
	int				err_no;
	t_parser_func	parser_functions[15];

	parser_functions[0] = parser_noop;
	parser_functions[1] = parser_noop;
	parser_functions[2] = parser_noop;
	parser_functions[3] = NULL;
	parser_functions[4] = parser_noop;
	parser_functions[5] = parser_noop;
	parser_functions[6] = parser_noop;
	parser_functions[7] = parser_redir;
	parser_functions[8] = parser_redir;
	parser_functions[9] = parser_add_new_arg;
	parser_functions[10] = parser_add_new_arg;
	parser_functions[11] = parser_add_env_var;
	parser_functions[12] = parser_add_new_arg;
	parser_functions[13] = parser_noop;
	parser_functions[14] = parser_arith_expan;
	if (parser_functions[token->id])
	{
		err_no = parser_functions[token->id](cmd, token);
		if (err_no)
			return (err_no);
	}
	return (PARSING_OK);
}

static int	traverse_tokens_to_make_cmd(t_cmd *cmd, t_token *start_token,
	t_token *end_token)
{
	int	err_no;

	while (start_token)
	{
		err_no = build_command_from_token(cmd, start_token);
		if (err_no)
			return (ERR_CMD);
		if (start_token == end_token)
			break ;
		if (start_token->id == LT || start_token->id == GT)
		{
			start_token = start_token->next;
			if (start_token == end_token)
				break ;
			start_token = start_token->next;
			if (start_token->id == PIPE)
				break ;
		}
		else if (start_token->id == ARITH_EXPAN)
			start_token = get_after_arith_expan_token(start_token);
		else
			start_token = start_token->next;
	}
	return (PARSING_OK);
}

void	make_cmd(t_shell *shell, t_cmd **cmd, t_token *start_token, \
	t_token *end_token)
{
	size_t	arg_num;
	int		err_no;

	*cmd = new_cmd();
	if (!*cmd)
		handle_parsing_err(shell, ERR_CMD);
	arg_num = get_arg_num(start_token, end_token);
	(*cmd)->args = ft_calloc((arg_num + 1), sizeof(char *));
	if (!(*cmd)->args)
		handle_parsing_err(shell, ERR_CMD);
	err_no = traverse_tokens_to_make_cmd(*cmd, start_token, end_token);
	if (err_no)
		handle_parsing_err(shell, err_no);
}

int	parse(t_shell *shell)
{
	int	err_no;

	err_no = tokenize(shell, shell->input);
	if (err_no)
		return (err_no);
	err_no = syntax(shell);
	if (err_no)
		return (err_no);
	err_no = append(shell);
	if (err_no)
		return (err_no);
	err_no = handle_heredocs(shell, shell->token);
	if (err_no)
		return (err_no);
	if (g_signalcode == SIGINT)
		return (SIGINT_HDOC);
	shell->tree = make_tree(shell, shell->token, last_token(shell->token));
	if (!shell->tree)
		return (ERR_MEM);
	//print_tree(shell->tree, 0);
	return (PARSING_OK);
}
