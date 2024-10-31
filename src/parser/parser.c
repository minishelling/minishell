#include "../../include/minishell.h"

t_ecode_p			parse(t_shell *shell);
void 				make_cmd(t_shell *shell, t_cmd **cmd, \
	t_token *start_token, t_token *end_token);
static size_t		get_arg_num(t_token *start_token, t_token *end_token);
static t_ecode_p	traverse_tokens_to_make_cmd(t_cmd *cmd, \
	t_token *start_token, t_token *end_token);
static t_ecode_p	build_command_from_token(t_cmd *cmd, t_token *token);

/**
 * @brief Parses the shell input and constructs the command tree.
 *
 * This function tokenizes the input, performs syntax checks, appends 
 * the tokens, handles here documents, and constructs the command 
 * tree from the tokens.
 *
 * @param shell Pointer to the shell structure containing input.
 * @return t_ecode_p PARSING_OK if successful, or an error code if any 
 *         issue is detected.
 */
t_ecode_p	parse(t_shell *shell)
{
	t_ecode_p	err_no;

	err_no = tokenize(shell, shell->input);
	if (err_no)
		return (err_no);
	err_no = syntax(shell);
	if (err_no)
		return (err_no);
	err_no = append(shell);
	if (err_no)
		return (err_no);
	if (shell->token && shell->debug_mode == ON)
		print_tokens(shell->token);
	err_no = handle_hdocs(shell, shell->token);
	if (err_no)
		return (err_no);
	if (g_signalcode == SIGINT)
		return (SIGINT_HDOC);
	shell->tree = make_tree(shell, shell->token, last_token(shell->token));
	if (shell->tree && shell->debug_mode == ON)
		print_tree(shell->tree);
	return (PARSING_OK);
}

/**
 * @brief Creates a new command from the token list.
 *
 * Allocates memory for a new command and populates its arguments by 
 * traversing tokens from the start token to the end token. If any 
 * allocation fails, it handles the parsing error appropriately.
 *
 * @param shell Pointer to the shell structure.
 * @param cmd Double pointer to the command structure to be created.
 * @param start_token Pointer to the starting token for command creation.
 * @param end_token Pointer to the ending token for command creation.
 */
void	make_cmd(t_shell *shell, t_cmd **cmd, t_token *start_token, \
	t_token *end_token)
{
	size_t		arg_num;
	t_ecode_p	err_no;

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

/**
 * @brief Counts the number of arguments in a token list.
 *
 * Traverses the tokens from the start token to the end token, counting
 * valid argument tokens while adjusting for redirection tokens. An
 * argument is added for tokens of type `WORD`, `SQUOTE`, `DQUOTE`, or
 * `ENV_VAR` (if non-empty, indicating a successful expansion). 
 * Redirection tokens (`LT` and `GT`) are followed by a target, typically 
 * a filename or, in the case of a heredoc, a file descriptor. Since the 
 * target is a `WORD` token, the count is incremented when found, but 
 * it does not need to be part of the command arguments, so it is 
 * subsequently decremented.
 *
 * @param start_token Pointer to the starting token of the list.
 * @param end_token Pointer to the ending token of the list.
 * @return The count of arguments found between `start_token` and `end_token`.
 */
static size_t	get_arg_num(t_token *start_token, t_token *end_token)
{
	size_t	arg_count;
	t_token	*cur_token;

	arg_count = 0;
	cur_token = start_token;
	while (cur_token)
	{
		if ((cur_token->id == ENV_VAR && *(cur_token->str) != '\0') \
			|| cur_token->id == SQUOTE || cur_token->id == DQUOTE \
			|| cur_token->id == WORD)
			arg_count++;
		if (cur_token->id == LT || cur_token->id == GT)
			arg_count--;
		if (cur_token == end_token)
			break ;
		cur_token = cur_token->next;
	}
	return (arg_count);
}

/**
 * @brief Traverses tokens to build a command from the token list.
 *
 * This function iterates through the tokens from start_token to end_token,
 * calling the appropriate function to build the command based on the token's
 * type. It handles redirection tokens and arithmetic expansion tokens.
 *
 * @param cmd Pointer to the command structure to fill.
 * @param start_token Pointer to the starting token of the list.
 * @param end_token Pointer to the ending token of the list.
 * @return Error code indicating the result of the operation.
 */
static t_ecode_p	traverse_tokens_to_make_cmd(t_cmd *cmd, \
	t_token *start_token, t_token *end_token)
{
	t_ecode_p	err_no;

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
			start_token = get_after_matching_arith_expan(start_token);
		else
			start_token = start_token->next;
	}
	return (PARSING_OK);
}

/**
 * @brief Builds a command from a token based on its type.
 *
 * This function uses a parser function array to call the appropriate
 * function to handle the command-building process based on the token's
 * ID. It assumes that any ampersand tokens present in the input
 * resulted in a syntax error in the previous syntax stage. Additionally,
 * tokens such as PIPE, AND_OPR, OR_OPR, PAR_OPEN, PAR_CLOSE, and spacing 
 * tokens are not present between start_token and end_token since they 
 * have already been used for creating the AST and dividing the commands, 
 * and thus will not trigger any function in this context. A new token, 
 * ARITH_EXPAN, has been added to handle arithmetical expansions without 
 * executing the statements inside of it. Furthermore, this function adds 
 * an argument for tokens of type WORD, SQUOTE, DQUOTE, and ENV_VAR.
 *
 * @param cmd Pointer to the command structure to be populated.
 * @param token Pointer to the token that represents the command part.
 * @return Error code indicating the result of the operation.
 */
static t_ecode_p	build_command_from_token(t_cmd *cmd, t_token *token)
{
	t_ecode_p		err_no;
	t_parser_func	make_cmd[16];

	make_cmd[7] = parser_redir;
	make_cmd[8] = parser_redir;
	make_cmd[9] = parser_add_new_arg;
	make_cmd[10] = parser_add_new_arg;
	make_cmd[11] = parser_add_env_var;
	make_cmd[12] = parser_add_new_arg;
	make_cmd[15] = parser_arith_expan;
	if (make_cmd[token->id])
	{
		err_no = make_cmd[token->id](cmd, token);
		if (err_no)
			return (err_no);
	}
	return (PARSING_OK);
}
