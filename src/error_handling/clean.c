/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfeuer <tfeuer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:42:49 by tfeuer            #+#    #+#             */
/*   Updated: 2024/10/31 13:42:50 by tfeuer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	clean_nicely(t_shell *shell);
void	clean_nicely_and_exit(t_shell *shell, int exit_code);

void	clean_nicely(t_shell *shell)
{
	if (shell->input)
	{
		free(shell->input);
		shell->input = NULL;
	}
	if (shell->token)
		free_token_list(&shell->token);
	if (shell->tree)
		free_tree(&shell->tree);
}

void	clean_nicely_and_exit(t_shell *shell, int exit_code)
{
	clean_nicely(shell);
	free_env_list(&shell->env_list);
	exit (exit_code);
}
