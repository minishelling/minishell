#include "../../include/minishell.h"

size_t	list_token_size(t_token *t_list)
{
	size_t	ret;

	ret = 0;
	if (t_list == NULL)
		return (0);
	while (t_list != NULL)
	{
		t_list = t_list->next;
		ret++;
	}
	return (ret);
}

void	list_token_print(t_token *head)
{
	int			size;
	t_token		*ptr;
	const int	con = list_token_size(head);
	const char	*token_name[14] = {
	[0] = "SPACE_CHAR",
	[1] = "TAB_CHAR",
	[2] = "NL",
	[3] = "PIPE",
	[4] = "AND_OPR",
	[5] = "SEMI_COL",
	[6] = "PAR_OPEN",
	[7] = "PAR_CLOSE",
	[8] = "GT",
	[9] = "LT",
	[10] = "SQUOTE",
	[11] = "DQUOTE",
	[12] = "env_var",
	[13] = "WORD"
	};
	ptr = head;

	size = list_token_size(ptr);
	printf("\nLIST OF TOKENS [%d] \n", con);
	printf("-------------------\n");
	while (size--)
	{
		printf("TOKEN [%02d]\tid: %s [%d]\n\t\tstr: |%s|\n", (con - size), token_name[ptr->id], ptr->id, ptr->str);
		ptr = ptr->next;
	}
	return ;
}

