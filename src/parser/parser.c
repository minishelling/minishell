#include "../../include/minishell.h"


size_t get_arg_num(t_token *start_token, t_token *end_token)
{
	size_t	arg_count;
	t_token	*cur_token;

	arg_count = 0;
	cur_token = start_token;
	while (cur_token)
	{
		if (cur_token->id == WORD || cur_token->id == ENV_VAR || cur_token->id == SQUOTE || cur_token->id == DQUOTE)
			arg_count++;
		if (cur_token == end_token)
			break;
		cur_token = cur_token->next;
	}
	return (arg_count);
}

typedef int (*t_parser_func)(t_cmd *current_cmd, t_token *token);

int	build_command_from_token(t_cmd *current_cmd, t_token *token)
{
int err_no;
t_parser_func parser_functions[15] = {
	[0] = parser_noop,
	[1] = parser_noop,
	[2] = parser_noop,
	[3] = NULL,
	[4] = parser_noop,
	[5] = parser_noop,
	[6] = parser_noop,
	[7] = parser_redir,
	[8] = parser_redir,
	[9] = parser_add_new_arg,
	[10] = parser_add_new_arg,
	[11] = parser_add_new_arg,
	[12] = parser_add_new_arg,
	[13] = parser_noop,
	[14] = parser_arith_expan
	};
	if (parser_functions[token->id])
	{
		err_no = parser_functions[token->id](current_cmd, token);
		if (err_no) 
			return (err_no);
	}
	return (PARSING_OK);
}


static int	traverse_tokens_to_make_cmd(t_cmd *current_cmd, t_token *start_token, t_token *end_token)
{
	int err_no;
	t_token	*cur_token;
	
	cur_token = start_token;
	while (cur_token)
	{
		// printf (" !!  CUR TOKEN is %s\n", cur_token->str);
		err_no = build_command_from_token(current_cmd, cur_token);
		if (cur_token == end_token)
			break;
		//printf ("end_token is %s\n", end_token->str);
		if (err_no)
			return (ERR_CMD);
		if (cur_token->id == LT || cur_token->id == GT)
            cur_token = get_after_word_token(cur_token);
        else if (cur_token->id == ARITH_EXPAN)
			cur_token = get_after_arith_expan_token(cur_token);
        else
            cur_token = cur_token->next;
	}
	return (PARSING_OK);
}

void	make_cmd(t_shell *shell, t_cmd **cmd, t_token *start_token, t_token *end_token)
{
	size_t	arg_num;
	int		err_no;
	
	*cmd = new_cmd();
	if (!*cmd)
	{
		handle_parsing_err(shell, ERR_CMD, NULL);
		clean_nicely_and_exit(shell, NULL);
	}
	arg_num = get_arg_num(start_token, end_token);
	(*cmd)->args = ft_calloc((arg_num + 1), sizeof(char *));
	if (!(*cmd)->args)
	{
		handle_parsing_err(shell, ERR_CMD, NULL);
		clean_nicely_and_exit(shell, NULL);
	}
	err_no = traverse_tokens_to_make_cmd(*cmd, start_token, end_token);
	if (err_no)
	{
		handle_parsing_err(shell, err_no, NULL);
		clean_nicely_and_exit(shell, NULL);
	}
}

int	parse(t_shell *shell)
{
	int err_no;

	err_no = tokenize(shell, shell->input);
	if (err_no)
		return (free_token_list(&shell->token), err_no);
	// printf ("After tokenization:\n");
	// print_token(shell->token);
	
	err_no = syntax(shell);
	if (err_no)
		return (free_token_list(&shell->token), err_no);
	// printf ("After syntax:\n");
	// print_token(shell->token);
	
	err_no = append(shell);
	if (err_no)
		return (free_token_list(&shell->token), err_no);
	// printf ("After append:\n");
	// print_token(shell->token);
	err_no = handle_heredocs(shell, shell->token);
	if (err_no)
	return (free_token_list(&shell->token), err_no);
	// printf ("after heredocs handling\n");
	// print_token(shell->token);
	
	//fprintf(stderr, "PARSER:\nSignal code: %d\n", g_signalcode);
	//fprintf(stderr, "Exit code: %d\n", shell->exit_code);
	if (g_signalcode == SIGINT)
		return (SIGINT_HDOC);

	shell->tree = make_tree(shell, shell->token, last_token(shell->token));
	if (!shell->tree)
		free_token_list(&shell->token);   //protect more
	// printf("\n"WHITE_TEXT MAGENTA_BACKGROUND"THE TREE"RESET_COLOR);
	// printf("\n--------------------\n");
	
	// if (shell->tree)
	// 	print_tree_verbose(shell->tree, 0);
	// printf ("\n");
	
	return (PARSING_OK);
}

