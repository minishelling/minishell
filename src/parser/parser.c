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

/**
 * @brief Get the number of arguments from a token.
 *
 * This function counts the number of arguments represented by a token,
 * taking into account different token types such as words, environment
 * variables, and quotes. It returns the count of arguments.
 *
 * @param token A pointer to the token to analyze.
 * @return The number of arguments represented by the token.
 */
size_t get_arg_num(t_token *token)
{
    size_t arg_count;

	arg_count = 0;
    while (token)
    {
        if (is_word(token) || is_env_var(token) || is_squote(token) || is_dquote(token))
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
 * @param shell Pointer to the shell structure.
 * @param current_cmd Pointer to the current command structure.
 * @param token Pointer to the current token.
 * @return true if processing is successful, false otherwise.
 */
static bool process_token(t_shell *shell, t_cmd *current_cmd, t_token *token)
{
    int status;
    t_parser_func parser_functions[16] = {
        [0] = parser_space,
        [1] = parser_space,
        [2] = parser_space,
        [3] = NULL,
        [4] = parser_and_opr,
        [5] = parser_semicol,
        [6] = parser_par_open,
        [7] = parser_par_close,
        [8] = parser_redir,
        [9] = parser_redir,
        [10] = add_new_arg, // for single quotes
        [11] = add_new_arg, // for double quotes
        [12] = add_new_arg, // for environment variable
        [13] = add_new_arg, // for general cases
        [14] = parser_or_opr,
        [15] = parser_arith_expan
    };
    if (parser_functions[token->id] != NULL) 
	{
        status = parser_functions[token->id](current_cmd, token);
        if (status) 
		{
            handle_parsing_err(shell, status, NULL);
            return (false);
        }
    }
    return (true);
}

/**
 * @brief Processes tokens in a command by calling the appropriate parser functions.
 *
 * @param shell Pointer to the shell structure.
 * @param current_cmd Pointer to the current command structure.
 * @param token Pointer to the first token.
 * @return true if all tokens are processed successfully, false otherwise.
 */
static bool process_tokens(t_shell *shell, t_cmd *current_cmd, t_token *token)
{
    while (token)
    {
        if (!(process_token(shell, current_cmd, token)))
            return (false);
        if (token->id == LT || token->id == GT)
            token = get_after_word_token(token);
        else if (token->id == ARITH_EXPAN)
            token = get_after_arith_expan_token(token);
        else
            token = get_after_space_token(token);
        if (current_cmd->next)
            return (false);
    }
    return (true);
}

/**
 * @brief Initializes a command by setting up arguments and processing tokens.
 *
 * @param shell Pointer to the shell structure.
 * @param current_cmd Pointer to the current command structure.
 * @param token Pointer to the first token.
 * @return true if initialization is successful, false otherwise.
 */
bool init_cmd(t_shell *shell, t_cmd **current_cmd, t_token *token)
{
    size_t arg_num = get_arg_num(token);
    (*current_cmd)->args = ft_calloc((arg_num + 1), sizeof(char *));
    if (!(*current_cmd)->args)
        return (false);
    if (!(process_tokens(shell, *current_cmd, token)))
    {
        free_cmd(*current_cmd);
        return (false);
    }
    return (true);
}


int	make_cmd(t_shell *shell, t_token *start_token, t_token *end_token)
{
	t_token	*token;

	shell->cmd = NULL;
	token = start_token;

	while (token)
	{
		shell->cmd = new_cmd();
		//printf ("made a new cmd \n");
		if (!init_cmd(shell, &shell->cmd, token))
		{
			free_token_list(shell->token);
			return (ERR_MEM);
		}

		// add_cmd_in_back(&shell->cmd, cmd);
		(void)end_token;
		// if (token == end_token)
		break;
	}
	//return (shell->cmd);
	return (PARSING_OK);
}

int	parse(t_shell *shell)
{
	int err_no;

	err_no = tokenize(shell, shell->input);
	if (err_no)
		return (free_token_list(shell->token), err_no);
	// printf ("After tokenization:\n");
	// print_token(shell->token);
	
	err_no = syntax(shell);
	if (err_no)
		return (free_token_list(shell->token), err_no);
	// printf ("After syntax:\n");
	// print_token(shell->token);
	err_no = join_quotes_tokens(shell);
	if (err_no)
		return (free_token_list(shell->token), err_no);
	// printf ("after joining quotes tokens:\n");
	// print_token(shell->token);
	
	err_no = join_word_and_env_var_tokens(shell);
	if (err_no)
	return (free_token_list(shell->token), err_no);
	// printf ("after concat words and env_vars tokens\n");
	// print_token(shell->token);
	
	remove_space_tokens(&shell->token);
	// printf ("after removing space tokens\n");
	// print_token(shell->token);

	remove_subshell_parens(shell);
	// printf ("after removing subshell_parens\n");
	// print_token(shell->token);

	handle_heredocs(shell->token);
	// printf ("after heredocs handling\n");
	// print_token(shell->token);
	if (g_exitcode == 130)
	{
		return (SIGINT_HDOC);
	}

	shell->tree = make_tree(shell, shell->token, last_token(shell->token));

	printf("\n"WHITE_TEXT MAGENTA_BACKGROUND"THE TREE"RESET_COLOR);
	printf("\n--------------------\n");
	
	if (shell->tree)
		print_tree_verbose(shell->tree, 0);
	
	// if (shell->tree)
	// 	print_tree_with_cmds(shell->tree, 0);
	//printf ("\n");
		
	// if (shell->tree)
	// 	free_token_list(shell->token); //???
	return (PARSING_OK);
}


