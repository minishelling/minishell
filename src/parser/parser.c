#include "../../include/minishell.h"

typedef t_token*	(*t_parser_func)(t_cmd *cmd, t_token *token);

size_t	get_arg_num(t_token *token)
{
	size_t	arg_num;

	arg_num = 0;
	if (token == NULL)
		return (0);
	while (token != NULL && token->id != PIPE)
	{
		if (token->id == WORD)
			arg_num++;
		if (token->id == GT || token->id == LT)
		{
			printf ("In getting args, token->id is %d\n", token->id);
			token = get_after_space_token(token);
			printf ("In getting args, token->id after space is %d\n", token->id);
		}
		token = get_after_space_token(token);
	}
	//printf ("arg_num is %ld\n", arg_num);
	return (arg_num);
}

bool	is_init_cmd(t_cmd *cmd, t_token *token)
{
	size_t		arg_num = get_arg_num(token);
	t_parser_func	func[15] = {
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
	[10] = NULL, //quotes after handling 
	[11] = NULL, //quotes after handling
	[12] = NULL, //env var after handling
	[13] = parser_word,
	[14] = parser_or_opr
	};

	cmd->args = ft_calloc(sizeof(char *), (arg_num + 1));
	if (!cmd->args)
		return (false);
	while (token != NULL)
	{
		token = func[token->id](cmd, token);
		// if (cmd->next != NULL)
		// 	return (free_cmd(cmd), false);
	}
	//printf ("manages to init cmd\n");
	return (true);
}

t_cmd	*make_cmd(t_shell *shell)
{
	t_token	*token;
	t_cmd	*current_cmd;
	t_cmd	*cmd_list_head;

	token = shell->token;
	cmd_list_head = NULL;
	while (token != NULL)
	{
		current_cmd = new_cmd();
		//printf ("made a new cmd \n");
		if (!current_cmd)
			return (free_token_list(shell->token,free_token_str), NULL);
		if (is_init_cmd(current_cmd, token) == false)
			return (free_token_list(shell->token, free_token_str), \
					free_cmd_list(cmd_list_head), NULL);

		add_cmd_in_back(&cmd_list_head, current_cmd);
		token = get_after_pipe_token(token);
	}
	//free_token_list(shell->token, free_token_non_word);
	return (cmd_list_head);
}

int	parse(t_shell *shell)
{
	shell->token = tokenize(shell->input);
	if (shell->token == NULL)
	{
		//error
		
	}
	print_token(shell->token);
	int status = syntax(shell);
	if (status)
	{
		//printf ("syntax error\n");
		// error(syntax_error, 258, shell->syntax->str);
		free_token_list(shell->token, free_token_str);
		return (status);
	}
	//env_var_print_linked_list (shell->env_list);
	//env_print_list (shell->env_list);  // lisandro
	shell->token = expand(shell->token, shell->env_list);
	print_token(shell->token);
	concat_word_tokens(shell);
	// if (concatenate_word_tokens(shell) == false)
	// 	return (free_token_list(shell->token, list_token_free_node_str),
	// 			error(append));
	
	shell->cmd_list = make_cmd(shell);
	print_cmd(shell->cmd_list);
	// if (shell->cmd_list == NULL)
	// 	error(parser);
	// shell->token = NULL;
	return (PARSING_OK);

}

