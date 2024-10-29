#include "../../../../include/minishell.h"

t_ecode_p	syntax_noop(t_token *prev_token, t_token *cur_token, \
	int *par_count);
t_ecode_p	syntax_and_opr(t_token *prev_token, t_token *cur_token, \
	int *par_count);
t_ecode_p	syntax_ampersand(t_token *prev_token, t_token *cur_token, \
	int *par_count);
t_ecode_p	syntax_or_opr(t_token *prev_token, t_token *cur_token, \
	int *par_count);
t_ecode_p	syntax_pipe(t_token *prev_token, t_token *cur_token, \
	int *par_count);

t_ecode_p	syntax_noop(t_token *prev_token, t_token *cur_token, \
	int *par_count)
{
	(void)prev_token;
	(void)cur_token;
	(void)par_count;
	return (PARSING_OK);
}

t_ecode_p	syntax_and_opr(t_token *prev_token, t_token *cur_token, \
	int *par_count)
{
	t_token	*next_token;

	(void)par_count;
	if (!prev_token)
		return (ERR_SYNTAX_AND);
	next_token = skip_whitespace_and_get_next_token(cur_token);
	if (!next_token)
		return (ERR_PARSING_ERROR);
	if (next_token->id == PIPE)
		return (ERR_SYNTAX_PIPE);
	if (next_token->id == OR_OPR)
		return (ERR_SYNTAX_OR);
	if (next_token->id == AND_OPR)
		return (ERR_SYNTAX_AND);
	if (next_token->id == PAR_CLOSE)
		return (ERR_SYNTAX_CLOSE_PAR);
	if (next_token->id == AMPERSAND)
		return (ERR_SYNTAX_AMPER);
	return (PARSING_OK);
}

t_ecode_p	syntax_ampersand(t_token *prev_token, t_token *cur_token, \
	int *par_count)
{
	t_token	*next_token;

	(void)par_count;
	if (!prev_token)
		return (ERR_SYNTAX_AMPER);
	next_token = skip_whitespace_and_get_next_token(cur_token);
	if (next_token)
	{
		if (next_token->id == PIPE)
			return (ERR_SYNTAX_PIPE);
		if (next_token->id == AND_OPR)
			return (ERR_SYNTAX_AND);
		if (next_token->id == OR_OPR)
			return (ERR_SYNTAX_OR);
		if (next_token->id == AMPERSAND)
			return (ERR_SYNTAX_AMPER);
	}
	return (PARSING_OK);
}

t_ecode_p	syntax_or_opr(t_token *prev_token, t_token *cur_token, \
	int *par_count)
{
	t_token	*next_token;

	(void)par_count;
	if (!prev_token)
		return (ERR_SYNTAX_OR);
	next_token = skip_whitespace_and_get_next_token(cur_token);
	if (!next_token)
		return (ERR_PARSING_ERROR);
	if (next_token->id == PIPE)
		return (ERR_SYNTAX_PIPE);
	if (next_token->id == OR_OPR)
		return (ERR_SYNTAX_OR);
	if (next_token->id == AND_OPR)
		return (ERR_SYNTAX_AND);
	if (next_token->id == PAR_CLOSE)
		return (ERR_SYNTAX_CLOSE_PAR);
	if (next_token->id == AMPERSAND)
		return (ERR_SYNTAX_AMPER);
	return (PARSING_OK);
}

t_ecode_p	syntax_pipe(t_token *prev_token, t_token *cur_token, int *par_count)
{
	t_token	*next_token;

	(void)par_count;
	if (!prev_token)
		return (ERR_SYNTAX_PIPE);
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
	if (next_token->id == PAR_CLOSE)
		return (ERR_SYNTAX_CLOSE_PAR);
	if (next_token->id == AMPERSAND)
		return (ERR_SYNTAX_AMPER);
	return (PARSING_OK);
}
