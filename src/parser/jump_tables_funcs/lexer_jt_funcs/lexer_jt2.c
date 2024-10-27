#include "../../../../include/minishell.h"

int	advance_pos_quote(char *str, size_t *pos, t_token_id *token_id)
{
	(*pos)++;
	while (str[*pos] && *token_id != get_token_id(str[*pos]))
		(*pos)++;
	if (str[*pos])
		(*pos)++;
	return (PARSING_OK);
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
