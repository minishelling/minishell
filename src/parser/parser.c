#include "../../include/minishell.h"

/**
 * @brief Check if the token is a double quote.
 *
 * This function determines if the given token is classified as a
 * double quote based on its token ID.
 *
 * @param token A pointer to the token to check.
 * @return true If the token is a double quote.
 * @return false If the token is not a double quote.
 */
bool is_dquote(t_token *token)
{
	return (token && token->id == DQUOTE);
}

/**
 * @brief Check if the token is a single quote.
 *
 * This function determines if the given token is classified as a
 * single quote based on its token ID.
 *
 * @param token A pointer to the token to check.
 * @return true If the token is a single quote.
 * @return false If the token is not a single quote.
 */
bool is_squote(t_token *token)
{
	return (token && token->id == SQUOTE);
}

/**
 * @brief Check if the token is a word.
 *
 * This function determines if the given token is classified as a word
 * based on its token ID.
 *
 * @param token A pointer to the token to check.
 * @return true If the token is a word.
 * @return false If the token is not a word.
 */
bool is_word(t_token *token)
{
	return (token && token->id == WORD);
}

/**
 * @brief Check if the token is an environment variable.
 *
 * This function determines if the given token is classified as an
 * environment variable based on its token ID.
 *
 * @param token A pointer to the token to check.
 * @return true If the token is an environment variable.
 * @return false If the token is not an environment variable.
 */
bool is_env_var(t_token *token)
{
	return (token && token->id == ENV_VAR);
}

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
		// if (is_word(token) || is_env_var(token) || is_squote(token) || is_dquote(token))
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
 * @brief Processes a token with the appropriate parser function.
 *
 * For single quotes, double quotes, environment variables, and word tokens,
 * this function adds a new argument to the command structure.
 *
 * @param current_cmd Pointer to the current command structure.
 * @param token Pointer to the current token.
 * @return true if processing is successful, false otherwise.
 */
int	process_token(t_cmd *current_cmd, t_token *token)
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


static int	process_tokens(t_cmd *current_cmd, t_token *token)
{
	int err_no;
	
	while (token)
	{
		err_no = process_token(current_cmd, token);
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

int	init_cmd(t_cmd **current_cmd, t_token *token)
{
	int err_no;
	size_t arg_num;
	
	arg_num = get_arg_num(token);
	(*current_cmd)->args = ft_calloc((arg_num + 1), sizeof(char *));
	if (!(*current_cmd)->args)
		return (ERR_MEM);
	err_no = process_tokens(*current_cmd, token);
	if (err_no)
		return (err_no);
	return (PARSING_OK);
}

int	make_cmd(t_shell *shell, t_token *start_token)
{
	t_token	*token;
	int		err_no;

	shell->cmd = NULL;
	token = start_token;
	while (token)
	{
		shell->cmd = new_cmd();
		err_no = init_cmd(&shell->cmd, token);
		if (err_no)
		{
			free_cmd(&shell->cmd);
			free_token_list(shell->token);
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
		return (free_token_list(shell->token), err_no);
	printf ("After tokenization:\n");
	print_token(shell->token);
	
	err_no = syntax(shell);
	if (err_no)
		return (free_token_list(shell->token), err_no);
	printf ("After syntax:\n");
	print_token(shell->token);
	
	err_no = append (shell);
	if (err_no)
	return (free_token_list(shell->token), err_no);

	err_no = handle_heredocs(shell, shell->token);
	if (err_no)
	return (free_token_list(shell->token), err_no);
	printf ("after heredocs handling\n");
	
	print_token(shell->token);
	fprintf(stderr, "Parser:\nSignal code: %d\n", g_signalcode);
	fprintf(stderr, "Exit code: %d\n", shell->exit_code);
	if (g_signalcode == SIGINT)
	{
		return (SIGINT_HDOC);
	}

	shell->tree = make_tree(shell, shell->token, last_token(shell->token));

	printf("\n"WHITE_TEXT MAGENTA_BACKGROUND"THE TREE"RESET_COLOR);
	printf("\n--------------------\n");
	
	if (shell->tree)
		print_tree_verbose(shell->tree, 0);
	printf ("\n");
	// if (shell->tree)
	// 	free_token_list(shell->token); //???
	return (PARSING_OK);
}

