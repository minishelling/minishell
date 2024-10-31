#include "../../../../include/minishell.h"

t_ecode_p			parser_redir(t_cmd *cmd, t_token *token);
static t_redir_id	which_redir(char *str);
t_ecode_p			parser_add_env_var(t_cmd *cmd, t_token *token);
t_ecode_p			parser_arith_expan(t_cmd *cmd_node, t_token *token);
t_ecode_p			parser_add_new_arg(t_cmd *cmd, t_token *token);

/**
 * @brief Parses redirection tokens and adds them to the command.
 *
 * Processes a redirection token and associates it with the 
 * appropriate file. It checks for memory allocation errors and 
 * validates the syntax of the redirection file name before adding 
 * it to the command's redirection list.
 *
 * @param cmd Pointer to the command structure where the redirection 
 *            will be stored.
 * @param token Pointer to the redirection token being processed.
 *
 * @return 
 * - PARSING_OK if redirection is successfully parsed and added,
 * - ERR_MEM if memory allocation fails,
 * - ERR_SYNTAX_ERROR if the redirection file name is invalid 
 *   (e.g., starts with '|', '&', ';', '(', or ')').
 */
t_ecode_p	parser_redir(t_cmd *cmd, t_token *token)
{
	t_token	*file_token;
	t_redir	*redir_list;

	if (token->next)
		file_token = token->next;
	redir_list = new_redir();
	if (!redir_list)
		return (ERR_MEM);
	redir_list->redir_id = which_redir(token->str);
	redir_list->file = ft_strdup(file_token->str);
	if (!redir_list->file)
		return (ERR_MEM);
	if (redir_list->file[0] == '|' || redir_list->file[0] == '&' \
		|| redir_list->file[0] == ';' \
		|| redir_list->file[0] == '(' || redir_list->file[0] == ')')
		return (ERR_SYNTAX_ERROR);
	add_redir_in_back(&(cmd->redir), redir_list);
	return (PARSING_OK);
}

/**
 * @brief Determines the type of redirection based on the given string.
 *
 * This function analyzes a redirection string and identifies the 
 * corresponding redirection type (e.g., input, output, here-document).
 *
 * @param str The redirection string to analyze.
 *
 * @return 
 * - REDIR if no valid redirection is found,
 * - IN for input redirection ('<'),
 * - OUT for output redirection ('>'),
 * - APP for append output redirection ('>>'),
 * - HERE for here-document redirection ('<<').
 */
static t_redir_id	which_redir(char *str)
{
	if (str == NULL)
		return (0);
	if (str[0] == '<')
	{
		if (str[1] == '<')
			return (HERE);
		return (IN);
	}
	if (str[0] == '>')
	{
		if (str[1] == '>')
			return (APP);
		return (OUT);
	}
	return (REDIR);
}

/**
 * @brief Adds an environment variable to the command's arguments.
 *
 * This function appends the string from the provided token to the command's 
 * argument list. It first checks if the token string is empty. If not, 
 * it finds the next available index in the args array, duplicates the 
 * token string, and assigns it to that index. The last element of the 
 * args array is set to NULL to indicate the end of the arguments.
 *
 * @param cmd Pointer to the command structure to update.
 * @param token Pointer to the token containing the environment variable.
 * @return Error code indicating the result of the operation.
 */
t_ecode_p	parser_add_env_var(t_cmd *cmd, t_token *token)
{
	size_t	i;

	if ((*(token->str)) == '\0')
		return (PARSING_OK);
	i = 0;
	while (cmd->args[i])
		i++;
	cmd->args[i] = ft_strdup(token->str);
	if (!cmd->args[i])
		return (ERR_MEM);
	return (PARSING_OK);
}

/**
 * @brief Handles arithmetic expansion for a command.
 *
 * This function sets up unique arguments for arithmetic expansion by 
 * allocating space for two specific strings: "((" and "))". It ensures 
 * that if the command's existing arguments are not NULL, they are freed 
 * before allocating new space. The resulting arguments are designed to 
 * make execution recognize this special case and avoid executing any 
 * statements within the arithmetic expansion. Note that this token is 
 * not counted in the get_arg_num function.
 *
 * @param cmd_node Pointer to the command structure to update.
 * @param token Pointer to the token representing the arithmetic expansion.
 * @return Error code indicating the result of the operation.
 */
t_ecode_p	parser_arith_expan(t_cmd *cmd_node, t_token *token)
{
	(void)token;
	if (cmd_node->args)
	{
		free_args(&cmd_node->args);
		cmd_node->args = NULL;
	}
	cmd_node->args = ft_calloc(3, sizeof(char *));
	cmd_node->args[0] = ft_strdup("((");
	if (!cmd_node->args[0])
		return (ERR_MEM);
	cmd_node->args[1] = ft_strdup("))");
	if (!cmd_node->args[1])
		return (ERR_MEM);
	cmd_node->args[2] = NULL;
	return (PARSING_OK);
}

/**
 * @brief Adds a new argument to the command's argument list.
 *
 * Appends a new argument, based on the token's string value, to the 
 * command structure. This function is used for tokens of type 
 * `SQUOTE`, `DQUOTE`, and `WORD`, representing quoted or unquoted 
 * argument values.
 *
 * @param cmd Pointer to the command structure where the argument is 
 *            added.
 * @param token Pointer to the token containing the argument string.
 * @return Error code `PARSING_OK` if successful, or `ERR_MEM` if 
 *         memory allocation fails.
 */
t_ecode_p	parser_add_new_arg(t_cmd *cmd, t_token *token)
{
	size_t	i;

	i = 0;
	while (cmd->args[i])
		i++;
	cmd->args[i] = ft_strdup(token->str);
	if (!cmd->args[i])
		return (ERR_MEM);
	return (PARSING_OK);
}
