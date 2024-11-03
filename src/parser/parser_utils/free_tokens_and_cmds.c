/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_tokens_and_cmds.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfeuer <tfeuer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:25:56 by tfeuer            #+#    #+#             */
/*   Updated: 2024/10/31 13:25:57 by tfeuer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void		free_token_list(t_token **list);
void		free_token(t_token **token);
void		free_cmd(t_cmd **cmd);
void		free_args(char ***args);
static void	free_redir_list(t_redir **list);

/**
 * @brief Frees a linked list of tokens.
 *
 * Iteratively frees each token in the linked list by calling
 * `free_token` on each element and advances to the next token.
 *
 * @param list Pointer to the head of the token list.
 */
void	free_token_list(t_token **list)
{
	t_token	*next;

	if (!list || !(*list))
		return ;
	while (*list)
	{
		next = (*list)->next;
		free_token(list);
		*list = next;
	}
}

/**
 * @brief Frees a single token structure.
 *
 * Releases the memory allocated for the token's string, if present,
 * and the token itself.
 *
 * @param token Pointer to the token to be freed.
 */
void	free_token(t_token **token)
{
	if (!token || !*token)
		return ;
	if ((*token)->str)
	{
		free((*token)->str);
		(*token)->str = NULL;
	}
	free(*token);
	*token = NULL;
}

/**
 * @brief Frees a command structure, including its arguments and redirections.
 *
 * Frees the command's argument list and redirection list if they are present.
 *
 * @param cmd Pointer to the command structure to be freed.
 */
void	free_cmd(t_cmd **cmd)
{
	if (*cmd == NULL)
		return ;
	if (cmd && *cmd && (*cmd)->args)
		free_args(&(*cmd)->args);
	if ((*cmd)->redir)
		free_redir_list(&(*cmd)->redir);
	free(*cmd);
	*cmd = NULL;
}

/**
 * @brief Frees a list of argument strings.
 *
 * Iterates through the array of argument strings, freeing each one,
 * then frees the array itself.
 *
 * @param args Pointer to the array of argument strings to be freed.
 */
void	free_args(char ***args)
{
	int	i;

	if (!(*args))
		return ;
	i = 0;
	while ((*args)[i])
	{
		free((*args)[i]);
		(*args)[i] = NULL;
		i++;
	}
	free(*args);
	*args = NULL;
}

/**
 * @brief Frees a list of redirections.
 *
 * Iterates through each redirection node in the list, freeing the
 * associated file string, if present, and then the node itself.
 *
 * @param list Pointer to the head of the redirection list.
 */
static void	free_redir_list(t_redir **list)
{
	t_redir	*temp_redir;

	if (!list || !(*list))
		return ;
	while (*list)
	{
		temp_redir = *list;
		*list = (*list)->next;
		if (temp_redir->file)
		{
			free(temp_redir->file);
			temp_redir->file = NULL;
		}
		free(temp_redir);
	}
	*list = NULL;
}
