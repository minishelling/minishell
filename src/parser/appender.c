#include "../../include/minishell.h"




void process_and_remove_parens(t_shell *shell, t_token *current, t_token *word_token)
{
	t_token *token_after_par;
	t_token *token_before_par;
	
	token_after_par = word_token->next;
	token_before_par = previous_token_if_exists(shell->token, current);
	shell->token = remove_token_by_reference(shell->token, current);
	shell->token = remove_token_by_reference(shell->token, word_token);
	if (token_before_par && token_before_par->id == PAR_OPEN &&
		token_after_par && token_after_par->id == PAR_CLOSE)
			handle_arith_expan(&shell->token, &token_before_par, &token_after_par);
}

void check_and_process_parens(t_shell *shell, t_token **current_token)
{
	t_token *next_token = (*current_token)->next;

	while (next_token && next_token->id == WORD && next_token->next)
		next_token = next_token->next;

	if (next_token && next_token->id == PAR_CLOSE)
	{
		process_and_remove_parens(shell, *current_token, next_token);
		t_token *token_after_par = next_token->next;
		if (token_after_par)
			*current_token = token_after_par;
		else
			return;
	}
	else
		*current_token = (*current_token)->next;
}

void remove_subshell_parens(t_shell *shell)
{
	t_token *current_token;
	
	current_token = shell->token;
	while (current_token && current_token->next)
	{
		if (current_token->id == PAR_OPEN)
		{
			check_and_process_parens(shell, &current_token);
			if (!current_token)
				break;
		}
		else
			current_token = current_token->next;
	}
}

static	int join_strs_and_free_cur_token(t_token **prev_token, t_token **cur_token)
{
	char	*joined_str;
	t_token	*temp;
	
	temp = (*cur_token)->next;
	joined_str = ft_strjoin((*prev_token)->str, (*cur_token)->str);
	if (!joined_str)
		return (ERR_MEM);
	if (safe_assign_str(&(*prev_token)->str, joined_str) != SUCCESS)
		return(free(joined_str), ERR_MEM);
	free(joined_str);
	free_token(&(*cur_token));
	*cur_token = temp;
	(*prev_token)->next = *cur_token;
	return (PARSING_OK);
}


int	join_env_var_quotes_and_word_str(t_shell *shell)
{
	t_token	*cur_token;
	t_token	*prev_token;
	int		err_no;

	prev_token = shell->token;
	cur_token = shell->token->next;
	while (prev_token && cur_token)
	{
		if ((prev_token->id == WORD || prev_token->id == ENV_VAR ||
			prev_token->id == SQUOTE || prev_token->id == DQUOTE) &&
			(cur_token->id == WORD || cur_token->id == ENV_VAR ||
			cur_token->id == SQUOTE || cur_token->id == DQUOTE))
			{
				err_no = join_strs_and_free_cur_token(&prev_token, &cur_token);
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


int join_quotes_tokens(t_shell *shell)
{
	t_token	*cur_token;
	t_token	*prev_token;
	int		err_no;

	prev_token = shell->token;
	cur_token = shell->token->next;
	while (prev_token && cur_token)
	{
		if ((prev_token->id == SQUOTE && cur_token->id == SQUOTE) ||
			(prev_token->id == DQUOTE && cur_token->id == DQUOTE) ||
			(prev_token->id == SQUOTE && cur_token->id == DQUOTE) ||
			(prev_token->id == DQUOTE && cur_token->id == SQUOTE))
			{
				err_no = join_strs_and_free_cur_token(&prev_token, &cur_token);
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


int append (t_shell *shell)
{
	int err_no;
	
	err_no = join_quotes_tokens(shell);
	if (err_no)
		return (free_token_list(&shell->token), err_no);
	printf ("after joining quotes tokens:\n");
	print_token(shell->token);
	err_no = join_env_var_quotes_and_word_str(shell);
	if (err_no)
	return (free_token_list(&shell->token), err_no);
	printf ("after concat quote, words and env_vars tokens\n");
	print_token(shell->token);
	remove_space_tokens(&shell->token);
	printf ("after removing space tokens\n");
	print_token(shell->token);
	remove_subshell_parens(shell);
	// printf ("after removing subshell_parens\n");
	// print_token(shell->token);
	return (PARSING_OK);
}

