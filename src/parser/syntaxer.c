#include "../../include/minishell.h"

typedef int	(*t_syntax_func) (t_token *t_prev, t_token *t_cur, \
		t_env *env_list);

t_token	*skip_whitespace(t_token *list)
{
	if (list == NULL)
		return (NULL);
	if (list->id == SPACE_CHAR || list->id == TAB_CHAR || list->id == NL)
		return (list->next);
	return (list);
}

bool is_par_closed(t_token *token_list_head) 
{
	t_token *current_token;
	int paren_count;

	paren_count = 0;
	current_token = token_list_head;
	while (current_token) 
	{
		//printf("current token is %s\n", current_token->str);
		if (current_token->id == PAR_OPEN) 
			paren_count++;

		else if (current_token->id == PAR_CLOSE) 
		{
			if (paren_count == 0) 
				return 0;
			paren_count--;
		}
		current_token = current_token->next;
	}
	return (paren_count == 0);
}

bool	is_quote_closed(t_token *token_list_head)
{
	t_token *current_token;
	char	*str;
	size_t	len;
	
	current_token = token_list_head;
	while (current_token) 
	{
		if (current_token->id == DQUOTE || current_token->id == SQUOTE)
		{
			len = ft_strlen(current_token->str);
			str = current_token->str;
			if (len == 1 || str[0] != str[len - 1])
			{
				printf("str[0] is %c and quote is unclosed\n", str[0]);
				//error 258, "unclosed quotes";
				return (false);
			}
		}
		current_token = current_token->next;
	}	
	return (true);
}

void	tokenize_and_n_or_opr(t_token *token_list_head)
{
	t_token *current_token;

	current_token = token_list_head;
	while (current_token)
	{
		if (!current_token->str || current_token->str[0] == '\0')
			return ;
		if (current_token->str[0] == '&' && current_token->str[1] != '&')
		// if (!ft_strncmp(current_token->str, "&", 1))
			current_token->id = WORD;
		else if (!ft_strncmp(current_token->str,"||", 2))
			current_token->id = OR_OPR;
		current_token = current_token->next;
	}
}
bool is_redir_before_parens(t_token *head)
{
    t_token *current = head;

    while (current && current->next)
    {
        // Check if current token is a redirection (LT or GT)
        if (current->id == LT || current->id == GT)
        {
            // Check if the next token is PAR_OPEN
            if (current->next->id == PAR_OPEN)
            {
                return 1;
            }
        }
        
        // Move to the next token
        current = current->next;
    }
	return 0;
}

int is_cmds_in_subshell_after_pipe(t_token *head)
{
    t_token *current = head;

    while (current && current->next)
    {
 
		// Look for a PIPE followed by PAR_OPEN
        if (current->id == PIPE && get_after_space_token(current)->id == PAR_OPEN)
        {
            t_token *next_token = current->next->next; // Move past PAR_OPEN
  
			printf ("I'm here\n");
            // Traverse tokens inside the parentheses
            while (next_token)
            {
				printf ("next token is %s\n", next_token->str);
                // If we encounter LOG_AND or LOG_OR inside the parentheses
                if (next_token->id == AND_OPR || next_token->id == OR_OPR)
                    return 1; // Throw the error
                next_token = next_token->next;
            }
        }
        // Move to the next token in the list
        current = current->next;
    }
    return 0; // Return 0 if no errors were found
}


int	syntax(t_shell *shell)
{
	t_token		*previous_token;
	t_token		*current_token;
	bool 		quote_closed;
	bool 		par_closed;
	int 		status;
	
	if (!shell->token) 
		return (0);
	
	quote_closed = is_quote_closed(shell->token);
	if (!quote_closed)
		return (ERR_UNCLOSED_QUOTES);
	par_closed = is_par_closed(shell->token);
	if (!par_closed)
	{
		printf ("not closed\n");
		return (ERR_UNCLOSED_PAREN);
	}
	if (is_redir_before_parens(shell->token))
	{
		printf ("redir before parens\n");
		return (ERR_SYNTAX_ERROR);
	}
	
	tokenize_and_n_or_opr(shell->token);

	if (is_cmds_in_subshell_after_pipe(shell->token))
		return (ERR_OUT_OF_SCOPE);
	
	
	
	
	
	t_syntax_func	func[15] = {
	[0] = syntax_id_misc,
	[1] = syntax_id_misc,
	[2] = syntax_id_misc,
	[3] = syntax_id_pipe,
	[4] = syntax_id_and_opr,
	[5] = syntax_id_semicol,
	[6] = syntax_id_parentheses, //parentheses
	[7] = syntax_id_parentheses, //parentheses
	[8] = syntax_id_redir,
	[9] = syntax_id_redir,
	[10] = syntax_id_misc,
	[11] = syntax_id_misc,
	[12] = syntax_id_misc,
	[13] = syntax_id_word, //word
	[14] = syntax_id_pipe // or_opr
	};

	previous_token = NULL;
	current_token = skip_whitespace(shell->token);
	while (current_token != NULL)
	{
		// printf ("in syntax, current_token->id is %d\n", current_token->id);
		status = (func[current_token->id](previous_token, current_token, shell->env_list));
		// printf ("status is %d\n", status);
		if (status)	
			return (status);
		previous_token = current_token;
		current_token = get_after_space_token(current_token);
	}
	return (0);
}

