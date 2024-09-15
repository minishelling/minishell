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

// t_token *remove_subshell_parens(t_token *head)
// {
// 	t_token *current = head;

// 	while (current && current->next && current->next->next)
// 	{
// 		// Check if the pattern PAR_OPEN -> WORD -> PAR_CLOSE exists
// 		if (current->id == PAR_OPEN && current->next->id == WORD && current->next->next->id == PAR_CLOSE)
// 		{
// 			// Remove the PAR_OPEN token
// 			head = remove_token(head, current);
			
// 			// Remove the PAR_CLOSE token
// 			head = remove_token(head, current->next->next);
			
// 			// Move current to the next token after the WORD token
// 			current = current->next;
// 		}
// 		else
// 		{
// 			// Move to the next token in the list
// 			current = current->next;
// 		}
// 	}
// 	return head;
// }

t_token *find_previous(t_token *head, t_token *target)
{
    t_token *current = head;
    while (current && current->next != target)
    {
        current = current->next;
    }
    return current;
}


t_token *handle_arith_expan(t_token **head, t_token *cur_open, t_token *cur_close)
{

	t_token *outer_open;
	t_token *outer_close;
	printf ("in arith expan\n");
	printf (">cur_open is %s , cur_close is %s\n", cur_open->str, cur_close->str);
	while (cur_open && cur_close && cur_open->id == PAR_OPEN && cur_close->id == PAR_CLOSE)
	{
		//printf ("I'm in the while loop\n");
	
		outer_open = cur_open;
		outer_close = cur_close;
		//printf (">outer_open is %s , outer_close is %s\n", outer_open->str, outer_close->str);
		cur_open = find_previous(*head, cur_open);
        cur_close = cur_close->next;
		// if (cur_open && cur_close)
			//printf (">cur_open is %s , cur_close is %s\n", cur_open->str, cur_close->str);
	}
	//printf ("outer open is %s , outer close %s\n", outer_open->str, outer_close->str);
	outer_open->id = ARITH_EXPAN;
	outer_open->str = "((";
	outer_close->id = ARITH_EXPAN;
	outer_close->str = "))";
    //printf("after handling arith expan:\n");
    //print_token(*head);
    return (*head);
}

t_token *remove_subshell_parens(t_token **head)
{
    t_token *current = *head;
	t_token *closing_par;

    while (current && current->next)
    {
        // Check if the pattern starts with PAR_OPEN
        if (current->id == PAR_OPEN)
        {
            t_token *word_token = current->next;

            // Loop through to find WORD tokens until PAR_CLOSE
            while (word_token && word_token->id == WORD)
            {
                word_token = word_token->next;
            }

            // Check if we reached PAR_CLOSE after all WORD tokens
            if (word_token && word_token->id == PAR_CLOSE)
            {
                // Remove the PAR_OPEN and PAR_CLOSE tokens
				closing_par = word_token->next;
                *head = remove_token(*head, current); // Remove PAR_OPEN
                *head = remove_token(*head, word_token); // Remove PAR_CLOSE

                // Move current to the first WORD token in the middle
                current = current->next;
                //printf("after removing subshells:\n");
               	//print_token(*head);

				if (find_previous(*head, current) && closing_par)
					//printf ("find_previous is %s, closing_par is %s\n", find_previous(*head, current)->str, closing_par->str );
                // Check for arithmetic expansion and handle it
                if (find_previous(*head, current) && find_previous(*head, current)->id == PAR_OPEN &&
                    closing_par && closing_par->id == PAR_CLOSE)
                {
                    handle_arith_expan(head, find_previous(*head, current), closing_par);
                }

                //printf("after arith expan head is %p\n", *head);
            }
            else
            {
                // If the pattern is incomplete, move current to the next token
                current = current->next;
            }
        }
        else
        {
            current = current->next;
        }

        // Debugging print statement
        // if (current)
            // printf("current is %s\n", current->str);
    }

    return (*head);
}
