#include "../../../include/minishell.h"

void	set_pos_end_quote(char *str, size_t *pos, t_token_id token_id)
{
	(*pos)++;
	while (str[*pos] && token_id != get_token_id(str[*pos]))
		(*pos)++;
	if (str[*pos])
		(*pos)++;
}

void	set_pos_end_pipe(char *str, size_t *pos, t_token_id token_id)
{
	int	i;

	(void) token_id;
	i = 0;
	while (i < 2 && str[*pos] && str[*pos] == '|')
	{
		(*pos)++;
		i++;
	}
}

void	set_pos_end_redir(char *str, size_t *pos, t_token_id token_id)
{
	char	c = str[*pos];
	int			i;

	(void) token_id;
	i = 0;
	while (i < 2 && str[*pos] && str[*pos] == c)
	{
		(*pos)++;
		i++;
	}
}

/*
** token_id_env_var
** first character of "name" should be alphabetic (isalpha) or underscore "_"
** rest of characters of "name" should be alphabetic
** or numeric (isalnum) or "_" underscore
*/
void	set_pos_end_env_var(char *str, size_t *pos, t_token_id token_id)
{
	(void) token_id;
	(*pos)++;

	//printf ("str is %s\n", str);
	if (str[*pos] == '?')
	{
		(*pos)++;
		return ;
	}
	if (str[*pos] && !ft_isalpha(str[*pos]) && str[*pos] != '_')
		return ;
	(*pos)++;
	while (str[*pos] && (ft_isalnum(str[*pos]) || str[*pos] == '_'))
		(*pos)++;
	//printf ("pos is now %zu\n", *pos);
}


void	set_pos_end_space_or_word(char *str, size_t *pos, t_token_id token_id)
{
	while (str[*pos] && token_id == get_token_id(str[*pos]))
		(*pos)++;
}

void	set_pos_end_and_opr(char *str, size_t *pos, t_token_id token_id)
{
	(void) token_id;

	if (str[*pos] == '&' && str[*pos + 1] && str[*pos +1] == '&')
		(*pos) += 2;
	else (*pos)++;	
}


void	set_pos_end_semicol(char *str, size_t *pos, t_token_id token_id)
{
	(void) token_id;

	while (str[*pos] && str[*pos] == ';')
	{
		(*pos)++;
	}
}

void	set_pos_end_parentheses(char *str, size_t *pos, t_token_id token_id)
{
	const char	c = str[*pos];
	(void) token_id;
	if (str[*pos] == c)
		(*pos)++;
}