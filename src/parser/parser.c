#include "../../include/minishell.h"

// /**
//  * @brief Get the number of arguments from a token.
//  *
//  * This function counts the number of arguments represented by a token,
//  * taking into account different token types such as words, environment
//  * variables, and quotes. It returns the count of arguments.
//  *
//  * @param token A pointer to the token to analyze.
//  * @return The number of arguments represented by the token.
//  */
size_t get_arg_num(t_token *token)
{
	size_t arg_count;

	arg_count = 0;
	while (token)
	{
		if (token->id == WORD || token->id == ENV_VAR || token->id == SQUOTE || token->id == DQUOTE)
			arg_count++;
		token = token->next;
	}
	return arg_count;
}

/**
 * @brief Type definition for a parser function.
 *
 * This type represents a function pointer that takes a command structure
 * and a token, returning an integer status.
 */
typedef int (*t_parser_func)(t_cmd *current_cmd, t_token *token);

/**
 * @brief Processes a token and applies the appropriate parsing function.
 * 
 * This function takes a token and passes it to the corresponding parser function 
 * based on its ID. The function array `parser_functions` handles different token 
 * types like redirections, arguments, and arithmetic expansions. By this point, 
 * pipe tokens have already been processed, and a new arithmetic expansion token 
 * (`arith_expan`) is supported.
 * 
 * @param current_cmd The current command being built.
 * @param token The token to process.
 * 
 * @return PARSING_OK if successful, or an error code if an error occurs.
 */
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


static int	assemble_command_tokens(t_cmd *current_cmd, t_token *token)
{
	int err_no;
	
	while (token)
	{
		err_no = build_command_from_token(current_cmd, token);
		if (err_no)
			return (err_no);
		if (token->id == LT || token->id == GT)
			token = get_after_word_token(token);
		else if (token->id == ARITH_EXPAN)
			token = get_after_arith_expan_token(token);
		else
			token = token->next;
	}
	return (PARSING_OK);
}
/**
 * @brief Initializes a command structure by allocating memory for arguments 
 * and processing tokens.
 *
 * This function takes a list of tokens, calculates the number of arguments,
 * allocates memory for the arguments, and processes the tokens to build the command.
 * 
 * @param current_cmd A pointer to the current command structure being initialized.
 * @param token The starting token from which the command will be built.
 * @return int Returns PARSING_OK (0) if successful, or an error code if memory 
 * allocation fails or token processing encounters an error.
 */
int	init_cmd(t_cmd **current_cmd, t_token *token)
{
	int err_no;
	size_t arg_num;
	
	arg_num = get_arg_num(token);
	(*current_cmd)->args = ft_calloc((arg_num + 1), sizeof(char *));
	if (!(*current_cmd)->args)
		return (ERR_MEM);
	err_no = assemble_command_tokens(*current_cmd, token);
	if (err_no)
		return (err_no);
	return (PARSING_OK);
}


int	make_cmd(t_shell *shell, t_cmd **cmd, t_token *start_token)
{
	t_token	*token;
	int		err_no;

	*cmd = NULL;
	token = start_token;
	while (token)
	{
		*cmd = new_cmd();
		err_no = init_cmd(cmd, token);
		if (err_no)
		{
			free_cmd(cmd);
			free_token_list(&shell->token);
			return (err_no);
		}
		break;
	}
    return (PARSING_OK);
}


int	parse(t_shell *shell)
{
	int err_no;

	err_no = tokenize(shell, shell->input);
	if (err_no)
		return (free_token_list(&shell->token), err_no);
	printf ("After tokenization:\n");
	print_token(shell->token);
	
	err_no = syntax(shell);
	if (err_no)
		return (free_token_list(&shell->token), err_no);
	printf ("After syntax:\n");
	print_token(shell->token);
	
	err_no = append(shell);
	if (err_no)
	return (free_token_list(&shell->token), err_no);

	err_no = handle_heredocs(shell, shell->token);
	if (err_no)
	return (free_token_list(&shell->token), err_no);
	printf ("after heredocs handling\n");
	
	//print_token(shell->token);
	//fprintf(stderr, "PARSER:\nSignal code: %d\n", g_signalcode);
	//fprintf(stderr, "Exit code: %d\n", shell->exit_code);
	if (g_signalcode == SIGINT)
	{
		return (SIGINT_HDOC);
	}

	shell->tree = make_tree(shell, shell->token, last_token(shell->token));
	if (!shell->tree)
		free_token_list(&shell->token);   //protect more
	printf("\n"WHITE_TEXT MAGENTA_BACKGROUND"THE TREE"RESET_COLOR);
	printf("\n--------------------\n");
	
	//printf ("token->str is at %p\n", shell->token->str);
	
	if (shell->tree)
		print_tree_verbose(shell->tree, 0);
	printf ("\n");
	
	return (PARSING_OK);
}

