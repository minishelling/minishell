#include "../../../include/minishell.h"

int	syntax_noop(t_token *prev_token, t_token *cur_token, t_env *env_list)
{
	(void) prev_token;
	(void) cur_token;
	(void) env_list;
	return (PARSING_OK);
}

int	syntax_pipe(t_token *prev_token, t_token *cur_token, t_env *env_list)
{
	t_token	*next_token;

	(void) env_list;
	next_token = skip_whitespace_and_get_next_token(cur_token);
	if (prev_token == NULL || next_token == NULL)
		return (ERR_SYNTAX_PIPE);
	if ((cur_token->id == PIPE || cur_token->id == OR_OPR) \
		&& next_token->id == PAR_CLOSE)
		return (ERR_SYNTAX_PIPE);
	if ((cur_token->id == PIPE || cur_token->id == OR_OPR) \
		&& (next_token->id == PIPE || next_token->id == OR_OPR))
		return (ERR_SYNTAX_PIPE);
	if ((cur_token->id == OR_OPR && (next_token->id == OR_OPR \
		|| next_token->id == PIPE)) \
		|| (cur_token->id == PIPE && next_token->id == OR_OPR))
		return (ERR_SYNTAX_PIPE);
	return (PARSING_OK);
}

int	syntax_and_opr(t_token *prev_token, t_token *cur_token, t_env *env_list)
{
	t_token	*next_token;

	(void) env_list;
	next_token = skip_whitespace_and_get_next_token(cur_token);
	if (prev_token == NULL || next_token == NULL)
		return (ERR_SYNTAX_AND);
	if (cur_token->id == AND_OPR && (prev_token->id == GT \
		|| prev_token->id == LT))
		return (ERR_SYNTAX_AND);
	if (cur_token->id == AND_OPR && (next_token->str[0] == '&' \
		|| next_token->id == OR_OPR || next_token->id == AND_OPR))
		return (ERR_SYNTAX_AND);
	if (cur_token->id == AND_OPR && next_token->id == PAR_CLOSE)
		return (ERR_SYNTAX_AND);
	return (PARSING_OK);
}

int	syntax_parens(t_token *prev_token, t_token *cur_token, t_env *env_list)
{
	t_token	*next_token;

	(void) prev_token;
	(void) env_list;
	(void) cur_token;
	next_token = skip_whitespace_and_get_next_token(cur_token);
	if (next_token)
	{
		if (cur_token->id == PAR_OPEN && next_token->id == AND_OPR)
			return (ERR_SYNTAX_AND);
		if (cur_token->id == PAR_OPEN && next_token->id == OR_OPR)
			return (ERR_SYNTAX_OR);
		if (cur_token->id == PAR_OPEN && next_token->id == PIPE)
			return (ERR_SYNTAX_PIPE);
	}
	return (PARSING_OK);
}

int	remove_delimiter_quotes(t_token *delimiter_token)
{
	delimiter_token->id = WORD;
	if (safe_assign_str(&(delimiter_token->str), \
		(ft_substr(delimiter_token->str, 1, \
			(ft_strlen(delimiter_token->str) - 2)))) != SUCCESS)
		return (ERR_MEM);
	return (PARSING_OK);
}

int	syntax_redir(t_token *prev_token, t_token *cur_token, t_env *env_list)
{
	t_token	*next_token;

	(void)env_list;
	(void) prev_token;
	next_token = skip_whitespace_and_get_next_token(cur_token);
	if (!next_token)
		return (ERR_SYNTAX_NL);
	if (next_token->id == PAR_OPEN)
		return (ERR_SYNTAX_UNEXPECT_OPEN);
	if (next_token->id == PAR_CLOSE)
		return (ERR_SYNTAX_UNEXPECT_CLOSE);
	if (next_token->id == PIPE || next_token->id == OR_OPR)
		return (ERR_SYNTAX_PIPE);
	if (next_token->id == AND_OPR)
		return (ERR_SYNTAX_AND);
	if (next_token->id == GT || next_token->id == LT)
		return (ERR_SYNTAX_REDIR);
	if (next_token->id == SQUOTE || next_token->id == DQUOTE)
		if (remove_delimiter_quotes(next_token) != SUCCESS)
			return (ERR_MEM);
	if (next_token->id != WORD)
		return (ERR_SYNTAX_ERROR);
	return (PARSING_OK);
}

int	syntax_word(t_token *prev_token, t_token *cur_token, t_env *env_list)
{
	t_token	*next_token;

	(void) prev_token;
	(void) env_list;
	(void) cur_token;
	next_token = skip_whitespace_and_get_next_token(cur_token);
	if (next_token)
	{
		if (cur_token->id == WORD && next_token->id == PAR_OPEN)
		{
			if (next_token->next == NULL)
				return (ERR_SYNTAX_NL);
			else
				return (ERR_SYNTAX_UNEXPECT_OPEN);
		}
	}
	return (PARSING_OK);
}
