#include "../../../include/minishell.h"

int	syntax_pipe(t_token *prev_token, t_token *current, t_env *env_list)
{
	t_token	*next_token;
	
	(void) env_list;
	next_token = get_after_space_token(current);
	if (prev_token == NULL || next_token == NULL)
		return (ERR_SYNTAX_PIPE);
	if ((current->id == PIPE || current->id == OR_OPR) && next_token->id == PAR_CLOSE)
		return (ERR_SYNTAX_PIPE);
	if ((current->id == PIPE || current->id == OR_OPR) && (next_token->id == PIPE || next_token->id == OR_OPR))
			return (ERR_SYNTAX_PIPE);
	if ((current->id == OR_OPR && (next_token->id == OR_OPR || next_token->id == PIPE)) ||
		(current->id == PIPE && next_token->id == OR_OPR))
			return (ERR_SYNTAX_PIPE);
	return (PARSING_OK);
}

int	syntax_redir(t_token *prev_token, t_token *current, t_env *env_list)
{
	t_token	*next_token;
	(void)env_list;
	(void) prev_token;
	next_token = get_after_space_token(current);
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
	if (next_token->id != WORD)
		return (ERR_SYNTAX_ERROR);
	return (PARSING_OK);
}

int	syntax_noop(t_token *t_prev, t_token *t_cur, t_env *env_list)
{
	(void) t_prev;
	(void) t_cur;
	(void) env_list;
	return (PARSING_OK);
}

int	syntax_parens(t_token *prev_token, t_token *current, t_env *env_list)
{
	t_token	*next_token;
	(void) prev_token;
	(void) env_list;
	(void) current;
	next_token = get_after_space_token(current);
	if (next_token)
	{
		if (current->id == PAR_OPEN && next_token->id == AND_OPR)
			return (ERR_SYNTAX_AND);
		if (current->id == PAR_OPEN && next_token->id == OR_OPR)
			return (ERR_SYNTAX_OR);
		if (current->id == PAR_OPEN && next_token->id == PIPE)
			return (ERR_SYNTAX_PIPE);
	}
	return (PARSING_OK);
}

int	syntax_word(t_token *prev_token, t_token *current, t_env *env_list)
{
	t_token	*next_token;
	(void) prev_token;
	(void) env_list;
	(void) current;
	next_token = get_after_space_token(current);
	if (next_token)
	{
		if (current->id == WORD && next_token->id == PAR_OPEN)
		{
			if (next_token->next == NULL)
				return (ERR_SYNTAX_NL);
			else
				return (ERR_SYNTAX_UNEXPECT_OPEN);
		}
	}
	return (PARSING_OK);
}
		

int	syntax_and_opr(t_token *prev, t_token *current, t_env *env_list)
{
	t_token	*next_token;

	(void) env_list;
	next_token = get_after_space_token(current);
	if (prev == NULL || next_token == NULL)
		return (ERR_SYNTAX_AND);
	if (current->id == AND_OPR && (prev->id == GT || prev->id == LT))
		return (ERR_SYNTAX_AND);
	if (current->id == AND_OPR && (next_token->str[0]== '&' || next_token->id == OR_OPR || next_token->id == AND_OPR))
		return (ERR_SYNTAX_AND);
	if (current->id == AND_OPR && next_token->id == PAR_CLOSE)
		return (ERR_SYNTAX_AND);
	return(PARSING_OK);
}
