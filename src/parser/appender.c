#include "../../include/minishell.h"

t_ecode_p	append(t_shell *shell);
t_ecode_p	join_quotes_tokens(t_shell *shell);
t_ecode_p	join_env_var_quotes_and_word_str(t_shell *shell);
t_ecode_p	join_strs_free_cur_token(t_token **prev_token, t_token **cur_token);
void		remove_space_tokens(t_token **list, t_token *prev);

t_ecode_p	append(t_shell *shell)
{
	t_ecode_p	err_no;

	err_no = join_quotes_tokens(shell);
	if (err_no)
		return (free_token_list(&shell->token), err_no);
	err_no = join_env_var_quotes_and_word_str(shell);
	if (err_no)
		return (free_token_list(&shell->token), err_no);
	remove_space_tokens(&shell->token, NULL);
	return (PARSING_OK);
}

t_ecode_p	join_quotes_tokens(t_shell *shell)
{
	t_token		*cur_token;
	t_token		*prev_token;
	t_ecode_p	err_no;

	prev_token = shell->token;
	cur_token = shell->token->next;
	while (prev_token && cur_token)
	{
		if ((prev_token->id == SQUOTE && cur_token->id == SQUOTE) \
			|| (prev_token->id == DQUOTE && cur_token->id == DQUOTE) \
			|| (prev_token->id == SQUOTE && cur_token->id == DQUOTE) \
			|| (prev_token->id == DQUOTE && cur_token->id == SQUOTE))
		{
			err_no = join_strs_free_cur_token(&prev_token, &cur_token);
			if (err_no)
				return (err_no);
		}
		else
		{
			prev_token = cur_token;
			cur_token = cur_token->next;
		}
	}
	return (PARSING_OK);
}

t_ecode_p	join_env_var_quotes_and_word_str(t_shell *shell)
{
	t_token		*prev_token;
	t_token		*cur_token;
	t_ecode_p	err_no;

	prev_token = shell->token;
	cur_token = shell->token->next;
	while (prev_token && cur_token)
	{
		if ((prev_token->id == WORD || prev_token->id == ENV_VAR \
		|| prev_token->id == SQUOTE || prev_token->id == DQUOTE) \
		&& (cur_token->id == WORD || cur_token->id == ENV_VAR \
		|| cur_token->id == SQUOTE || cur_token->id == DQUOTE))
		{
			err_no = join_strs_free_cur_token(&prev_token, &cur_token);
			if (err_no)
				return (err_no);
		}
		else
		{
			prev_token = cur_token;
			cur_token = cur_token->next;
		}
	}
	return (PARSING_OK);
}

t_ecode_p	join_strs_free_cur_token(t_token **prev_token, t_token **cur_token)
{
	char	*joined_str;
	t_token	*temp;

	temp = (*cur_token)->next;
	joined_str = ft_strjoin((*prev_token)->str, (*cur_token)->str);
	if (!joined_str)
		return (ERR_MEM);
	if (safe_assign_str(&(*prev_token)->str, joined_str) != SUCCESS)
		return (free(joined_str), ERR_MEM);
	free(joined_str);
	free_token(&(*cur_token));
	*cur_token = temp;
	(*prev_token)->next = *cur_token;
	return (PARSING_OK);
}

void	remove_space_tokens(t_token **list, t_token *prev)
{
	t_token	*current;
	t_token	*temp;

	current = *list;
	while (current != NULL)
	{
		if (current->id == SPACE_CHAR || current->id == TAB_CHAR \
			|| current->id == NL)
		{
			if (current->id == NL)
				ft_putstr_fd("We don't treat NL as metachars here.\n", 2);
			if (prev == NULL)
				*list = current->next;
			else
				prev->next = current->next;
			temp = current;
			current = current->next;
			free_token(&temp);
		}
		else
		{
			prev = current;
			current = current->next;
		}
	}
}
