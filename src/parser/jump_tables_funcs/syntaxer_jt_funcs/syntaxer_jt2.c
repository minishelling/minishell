#include "../../../../include/minishell.h"

int	syntax_pipe(t_token *prev_token, t_token *cur_token, int *par_count)
{
	t_token	*next_token;

	(void)par_count;
	next_token = skip_whitespace_and_get_next_token(cur_token);
	if (!prev_token)
		return (ERR_SYNTAX_PIPE);
	if (!next_token)
		return (ERR_PARSING_ERROR);
	if (next_token->id == PIPE)
		return (ERR_SYNTAX_PIPE);
	if (next_token->id == AND_OPR)
		return (ERR_SYNTAX_AND);
	if (next_token->id == OR_OPR)
		return (ERR_SYNTAX_OR);
	if (next_token->id == PAR_CLOSE)
		return (ERR_SYNTAX_CLOSE_PAR);
	if (next_token->id == PAR_CLOSE)
		return (ERR_SYNTAX_CLOSE_PAR);
	return (PARSING_OK);
}

static int	parens_count_check(t_token *cur_token, int *par_count)
{
	if (cur_token->id == PAR_OPEN)
		(*par_count)++;
	else if (cur_token->id == PAR_CLOSE)
		(*par_count)--;
	if (*par_count < 0)
		return (ERR_SYNTAX_CLOSE_PAR);
	return (PARSING_OK);
}

int	syntax_open_paren(t_token *prev_token, t_token *cur_token, int *par_count)
{
	t_token	*next_token;
	int		err_no;

	(void)prev_token;
	(void) cur_token;
	err_no = parens_count_check(cur_token, par_count);
	if (err_no)
		return (err_no);
	next_token = skip_whitespace_and_get_next_token(cur_token);
	if (!next_token)
		return (ERR_PARSING_ERROR);
	if (next_token->id == PIPE)
		return (ERR_SYNTAX_PIPE);
	if (next_token->id == AND_OPR)
		return (ERR_SYNTAX_AND);
	if (next_token->id == OR_OPR)
		return (ERR_SYNTAX_OR);
	if (next_token->id == PAR_CLOSE)
		return (ERR_SYNTAX_CLOSE_PAR);
	return (PARSING_OK);
}

int	syntax_close_paren(t_token *prev_token, t_token *cur_token, int *par_count)
{
	t_token	*next_token;
	int		err_no;

	(void) cur_token;
	err_no = parens_count_check(cur_token, par_count);
	if (err_no)
		return (err_no);
	if (!prev_token)
		return (ERR_SYNTAX_CLOSE_PAR);
	next_token = skip_whitespace_and_get_next_token(cur_token);
	if (next_token)
	{
		if (next_token->id == PAR_OPEN)
			return (ERR_SYNTAX_OPEN_PAR);
		if ((next_token->id == SQUOTE || next_token->id == DQUOTE \
			|| next_token->id == WORD))
			return (ERR_SYNTAX_ERROR);
	}
	return (PARSING_OK);
}
