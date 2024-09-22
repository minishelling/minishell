#include "../../include/minishell.h"

void	add_token_in_back(t_token **token_list_head, t_token *new_token)
{
	if (!new_token)
		return ;
	if (!(*token_list_head))
	{
		*token_list_head = new_token;
		return ;
	}
	last_token(*token_list_head)->next = new_token;
	return ;
}

t_token *remove_token(t_token *start_token, t_token *token_to_remove)
{
	printf ("start token is %s\n", start_token->str);
	t_token *current = start_token;
	t_token *prev = NULL;

	if (current == NULL || token_to_remove == NULL)
		return start_token;
	while (current)
	{
		if (current == token_to_remove)
		{
			if (prev == NULL)
				start_token = current->next;
			else
				prev->next = current->next;
			printf ("I'm freeing %p\n", current);
			free(current->str);
			free(current);
			return (start_token);
		}
		prev = current;
		current = current->next;
	}
	return (start_token);
}

void remove_space_tokens(t_token **head)
{
	t_token *current = *head;
	t_token *prev = NULL;
	t_token *temp = NULL;

	while (current != NULL)

		if (current->id == SPACE_CHAR )
		{
			if (prev == NULL)
				*head = current->next;
			else

				prev->next = current->next;
			temp = current;
			current = current->next;
			free(temp);
		}
		else
		{
			prev = current;
			current = current->next;
		}
}



t_token *find_previous(t_token *head, t_token *target)
{
	t_token *current;

	if (head == target)
		return (NULL);
  	current = head;
    while (current && current->next != target)
    {
        current = current->next;
    }
    return current;
}


t_token *handle_arith_expan(t_token **head, t_token **cur_open, t_token **cur_close)
{

	t_token *outer_open;
	t_token *outer_close;
	//printf ("in arith expan\n");
	//printf (">cur_open is %s , cur_close is %s\n", (*cur_open)->str, (*cur_close)->str);
	while (*cur_open && cur_close && (*cur_open)->id == PAR_OPEN && (*cur_close)->id == PAR_CLOSE)
	{
		//printf ("I'm in the while loop\n");
	
		outer_open = (*cur_open);
		outer_close = *cur_close;
		//printf (">outer_open is %s , outer_close is %s\n", outer_open->str, outer_close->str);
		(*cur_open) = find_previous(*head, (*cur_open));
        (*cur_close) = (*cur_close)->next;
		// if (cur_open && cur_close)
			//printf (">cur_open is %s , cur_close is %s\n", cur_open->str, cur_close->str);
	}
	//printf ("outer open is %s , outer close %s\n", outer_open->str, outer_close->str);
	outer_open->id = ARITH_EXPAN;
	outer_open->str = "((";
	outer_close->id = ARITH_EXPAN;
	outer_close->str = "))";
    // printf("after handling arith expan:\n");
    // print_token(*head);
    return (*head);
}


