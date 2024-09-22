#include "../../include/minishell.h"

typedef int	(*t_parser_func)(t_cmd *cmd, t_token *token);

size_t	get_arg_num(t_token *token)
{
	size_t	arg_num;

	arg_num = 0;
	if (token == NULL)
		return (0);
	while (token && token->id != PIPE)
	{
		if (token->id == WORD || token->id == SQUOTE || token->id == DQUOTE || token->id == ENV_VAR)
			arg_num++;
		if (token->id == GT || token->id == LT)
		{
			//printf ("In getting args, token->id is %d\n", token->id);
			token = get_after_space_token(token);
			//printf ("In getting args, token->id after space is %d\n", token->id);
		}
		token = get_after_space_token(token);
	}
	// printf ("arg_num is %ld\n", arg_num);
	return (arg_num);
}

bool	init_cmd(t_shell *shell, t_cmd **current_cmd, t_token *token)
{
	size_t		arg_num = get_arg_num(token);
	int			status;
	t_parser_func	func[16] = {
		[0] = parser_space,
		[1] = parser_space,
		[2] = parser_space,
		[3] = parser_pipe,
		[4] = parser_and_opr,
		[5] = parser_semicol,
		[6] = parser_par_open,
		[7] = parser_par_close,
		[8] = parser_redir,
		[9] = parser_redir,
		[10] = parser_word, //squotes
		[11] = parser_word, //dquotes
		[12] = parser_word, //env var
		[13] = parser_word,
		[14] = parser_or_opr,
		[15] = parser_arith_expan
	};
	(*current_cmd)->args = ft_calloc((arg_num + 1), sizeof(char *));
	if (!(*current_cmd)->args)
		return (false);
	while (token && token->id !=PIPE)
	{
		status = func[token->id](*current_cmd, token);
		// printf ("status now is %d\n", status);
		if (status)
			handle_error(shell, status, NULL);
		//printf ("token is %s\n", token->str);
		if (token->id == LT || token->id == GT)
			token = get_after_word_token(token);
		else if (token->id == ARITH_EXPAN)
			token = get_after_arith_expan_token(token);
		else
			token = get_after_space_token(token);
		if (token)
			//printf ("next token is %s\n", token->str);
		if ((*current_cmd)->next)
			return (free_cmd(*current_cmd), false);
	}
	// printf ("manages to init cmd\n");
	return (true);
}

t_cmd	*make_cmd(t_shell *shell, t_token *start_token, t_token *end_token)
{
	t_token	*token;
	t_cmd	*current_cmd;

	shell->cmd_list = NULL;
	token = start_token;

	while (token)
	{
		current_cmd = new_cmd();
		//printf ("made a new cmd \n");

		if (!init_cmd(shell, &current_cmd, token))
		{
			free_token_list(shell->token);
			free_cmd_list(shell->cmd_list);  //including args and redirs
			return (NULL);
		}

		add_cmd_in_back(&shell->cmd_list, current_cmd);
		//printf ("managed to add in back\n");

		if (token == end_token)
			break;  // Process the end_token, then break out of the loop

		token = get_after_pipe_token(token); // Move to next token after a pipe
	}

	return (shell->cmd_list);
}


int	parse(t_shell *shell)
{
	int status;

	shell->token = tokenize(shell->input);
	if (shell->token == NULL)
	{
		//error
	}
	// printf ("After tokenization:\n");
	// print_token(shell->token);
	
	status = syntax(shell);
	// printf ("After syntax:\n");
	// print_token(shell->token);
	if (status)
	{
		//printf ("syntax error\n");
		// error(syntax_error, 258, shell->syntax->str);
		//free_token_list(shell->token, free_token_str);
		free_token_list(shell->token);
		return (status);
	}
	//env_var_print_linked_list (shell->env_list);
	//env_print_list (shell->env_list);  // lisandro

	
	join_quotes_tokens(shell);
	// printf ("after joining quotes tokens:\n");
	// print_token(shell->token);
	
	join_word_and_env_var_tokens(shell);
	// if (join_word_and_env_var_tokens(shell) == false)
	// 	return (free_token_list(shell->token, list_token_free_node_str),
	// 			error(append));
	// printf ("after concat words and env_vars tokens\n");
	// print_token(shell->token);
	
	remove_space_tokens(&shell->token);
	// printf ("after removing space tokens\n");
	// print_token(shell->token);

	remove_subshell_parens(&(shell->token));
	// printf ("after removing subshell_parens\n");
	// print_token(shell->token);


	//mandatory:
	// status = make_cmd(shell, shell->token);
	// if (shell->cmd_list == NULL)
	// 	error(parser);
	// print_cmd(shell->cmd_list);
	// shell->token = NULL;
	// free_token_list(shell->token); //???

	//bonus:
	shell->tree = make_tree(shell, shell->token, last_token(shell->token));

	//printf("\n"WHITE_TEXT MAGENTA_BACKGROUND"THE TREE"RESET_COLOR);
	//printf("\n--------------------\n");
	
	// if (shell->tree)
	// 	print_tree(shell->tree, 0);
	
	// if (shell->tree)
	// 	print_tree_with_cmds(shell->tree, 0);
	//printf ("\n");
		
	// if (shell->tree)
	// 	free_token_list(shell->token); //???
	
	return (PARSING_OK);

}

