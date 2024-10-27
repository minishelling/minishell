#include "../../../include/minishell.h"

int	advance_pos_space_or_word(char *str, size_t *pos, t_token_id *token_id)
{
	while (str[*pos] && *token_id == get_token_id(str[*pos]))
		(*pos)++;
	return (PARSING_OK);
}


int	advance_pos_pipe(char *str, size_t *pos, t_token_id *token_id)
{
	int	pipe_count;

	(void)token_id;
	pipe_count = 0;
	while (pipe_count < 2 && str[*pos] && str[*pos] == '|')
	{
		(*pos)++;
		pipe_count++;
	}
	return (PARSING_OK);
}


int	advance_pos_and_operator(char *str, size_t *pos, t_token_id *token_id)
{
	(void) token_id;
	if (str[*pos] == '&' && str[*pos + 1] && str[*pos +1] == '&')
		(*pos) += 2;
	else
		(*pos)++;
	return (PARSING_OK);
}

int	advance_pos_parens(char *str, size_t *pos, t_token_id *token_id)
{
	char	c;

	(void) token_id;
	c = str[*pos];
	if (str[*pos] == c)
		(*pos)++;
	return (PARSING_OK);
}


int	advance_pos_redir(char *str, size_t *pos, t_token_id *token_id)
{
	int		redir_count;
	char	c;

	(void)token_id;
	c = str[*pos];
	redir_count = 0;
	while (redir_count < 2 && str[*pos] && str[*pos] == c)
	{
		(*pos)++;
		redir_count++;
	}
	return(PARSING_OK);
}


int	advance_pos_quote(char *str, size_t *pos, t_token_id *token_id)
{
	(*pos)++;
	while (str[*pos] && *token_id != get_token_id(str[*pos]))
		(*pos)++;
	if (str[*pos])
		(*pos)++;
	return(PARSING_OK);
}


int	advance_pos_env_var(char *str, size_t *pos, t_token_id *token_id)
{
	(void) token_id;
	(*pos)++;
	if (str[*pos] == '?')
	{
		(*pos)++;
		return (PARSING_OK);
	}
	if (str[*pos] == '\0')
	{
		*token_id = WORD;
		return (PARSING_OK);
	}
	if (str[*pos] == '(')
		return (ERR_CMD_SUBSTIT);
	if ((!ft_isalpha(str[*pos]) && str[*pos] != '_'))
		return (PARSING_OK);
	(*pos)++;
	while (str[*pos] && (ft_isalnum(str[*pos]) || str[*pos] == '_'))
		(*pos)++;
	return (PARSING_OK);
}
