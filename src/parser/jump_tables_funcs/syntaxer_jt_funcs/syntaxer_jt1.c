#include "../../../../include/minishell.h"

int	syntax_noop(t_token *prev_token, t_token *cur_token, int *par_count);
int	syntax_and_opr(t_token *prev_token, t_token *cur_token, int *par_count);
int	syntax_or_opr(t_token *prev_token, t_token *cur_token, int *par_count);
int	syntax_word(t_token *prev_token, t_token *cur_token, int *par_count);

int	syntax_noop(t_token *prev_token, t_token *cur_token, int *par_count)
{
	(void) prev_token;
	(void) cur_token;
	(void)par_count;
	return (PARSING_OK);
}

int	syntax_and_opr(t_token *prev_token, t_token *cur_token, int *par_count)
{
	t_token	*next_token;

	(void)par_count;
	next_token = skip_whitespace_and_get_next_token(cur_token);
	if (!prev_token)
		return (ERR_SYNTAX_AND);
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
	return (PARSING_OK);
}

int	syntax_or_opr(t_token *prev_token, t_token *cur_token, int *par_count)
{
	t_token	*next_token;

	(void)par_count;
	next_token = skip_whitespace_and_get_next_token(cur_token);
	if (!prev_token)
		return (ERR_SYNTAX_OR);
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
	return (PARSING_OK);
}

int	syntax_word(t_token *prev_token, t_token *cur_token, int *par_count)
{
	t_token	*next_token;

	(void)par_count;
	(void)prev_token;
	(void)cur_token;
	next_token = skip_whitespace_and_get_next_token(cur_token);
	if (next_token)
	{
		if (cur_token->id == WORD && next_token->id == PAR_OPEN)
		{
			if (next_token->next == NULL)
				return (ERR_SYNTAX_NL);
			else
				return (ERR_SYNTAX_ERROR);
		}
	}
	return (PARSING_OK);
}
