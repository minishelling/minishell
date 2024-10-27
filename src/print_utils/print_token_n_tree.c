#include "../../include/minishell.h"

size_t	token_size(t_token *t_list)
{
	size_t	ret;

	ret = 0;
	if (t_list == NULL)
		return (0);
	while (t_list)
	{
		t_list = t_list->next;
		ret++;
	}
	return (ret);
}

void	init_token_names(char **token_name)
{
	token_name[0] = "SPACE_CHAR";
	token_name[1] = "TAB_CHAR";
	token_name[2] = "NL";
	token_name[3] = "PIPE";
	token_name[4] = "AND_OPR";
	token_name[5] = "PAR_OPEN";
	token_name[6] = "PAR_CLOSE";
	token_name[7] = "GT";
	token_name[8] = "LT";
	token_name[9] = "SQUOTE";
	token_name[10] = "DQUOTE";
	token_name[11] = "ENV_VAR";
	token_name[12] = "WORD";
	token_name[13] = "OR_OPR";
	token_name[14] = "ARITH_EXPAN";
}

void	print_token(t_token *head)
{
	int			size;
	t_token		*ptr;
	int			con;
	char		*token_name[15];

	init_token_names(token_name);
	con = token_size(head);
	ptr = head;
	size = token_size(ptr);
	printf("\n"WHITE_TEXT MAGENTA_BACKGROUND"LIST OF TOKENS" \
	RESET_COLOR"\t[%d]\n", con);
	printf("--------------------\n");
	while (size--)
	{
		if (ptr && ptr->str)
		{
			printf(MAGENTA_TEXT"%pTOKEN [%02d]"RESET_COLOR"\t \
			id: %s [%d]\n\t\tstr: |%s|\tstr address: %p\n", ptr, \
			(con - size), token_name[ptr->id], ptr->id, ptr->str, ptr->str);
		}
		else
			printf("NULL\n");
		ptr = ptr->next;
	}
}

void	print_tree(t_tree *node, int level)
{
	int		i;
	char	*tree_node_name[4];

	tree_node_name[0] = "AND_OPR";
	tree_node_name[1] = "OR_OPR";
	tree_node_name[2] = "CMD";
	tree_node_name[3] = "PIPE";
	if (!node)
		return ;
	i = 0;
	while (i++ < level)
		printf("    ");
	if (node->type == CMD)
		printf(WHITE_TEXT MAGENTA_BACKGROUND "%s" RESET_COLOR \
		" start token:|%s|, end token:|%s|, tree address: %p\n", \
		tree_node_name[node->type], node->start_token->str, \
		node->end_token->str, node);
	else if (node->type == T_AND_OPR || node->type == T_OR_OPR \
		|| node->type == T_PIPE)
		printf(MAGENTA_TEXT"%s"RESET_COLOR" %p\n", \
		tree_node_name[node->type], node);
	else
		printf ("NULL");
	print_tree(node->left, level + 1);
	print_tree(node->right, level + 1);
}
