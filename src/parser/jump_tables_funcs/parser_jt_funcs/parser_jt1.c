/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_jt1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfeuer <tfeuer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:25:34 by tfeuer            #+#    #+#             */
/*   Updated: 2024/11/04 13:44:17 by tfeuer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../include/minishell.h"

t_ecode_p			parser_redir(t_cmd *cmd, t_token *token);
static t_redir_id	which_redir(char *str);
t_ecode_p			parser_add_env_var(t_cmd *cmd, t_token *token);
t_ecode_p			parser_arith_expan(t_cmd *cmd_node, t_token *token);
t_ecode_p			parser_add_new_arg(t_cmd *cmd, t_token *token);

/**
 * @brief Creates a new command from the token list.
 *
 * Allocates memory for a new command and populates its arguments by 
 * traversing tokens from the start token to the end token. If any 
 * allocation fails, it handles the parsing error appropriately. 
 * If an ambiguous redirect is encountered (indicated by a return 
 * value of 1 from the parser_redir function), it sets the shell's 
 * exit code to 1 without printing an additional error message.
 *
 * @param shell Pointer to the shell structure.
 * @param cmd Double pointer to the command structure to be created.
 * @param start_token Pointer to the starting token for command creation.
 * @param end_token Pointer to the ending token for command creation.
 */
t_ecode_p	parser_redir(t_cmd *cmd, t_token *token)
{
	t_token	*file_token;
	t_redir	*redir_list;

	file_token = token->next;
	if (*file_token->str == '\0')
	{
		ft_putstr_fd("Error: ambiguous redirect\n", 2);
		return (1);
	}
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
