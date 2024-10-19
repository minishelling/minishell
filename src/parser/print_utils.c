#include "../../include/minishell.h"

size_t	cmd_size(t_cmd *t_list)
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

size_t	token_size(t_token *t_list)
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

void	print_env(t_env *env_list)
{
	// printf("LINKED LISTOF ENV VARS:\n\n");
	while (env_list != NULL)
	{
		printf("%s", env_list->key);
		printf("=");
		printf("%s\n", env_list->value);
		env_list = env_list->next;
	}
}

void	print_token(t_token *head)
{
	int			size;
	t_token		*ptr;
	int	con = token_size(head);
	char	*token_name[15] = {
	[0] = "SPACE_CHAR",
	[1] = "TAB_CHAR",
	[2] = "NL",
	[3] = "PIPE",
	[4] = "AND_OPR",
	[5] = "PAR_OPEN",
	[6] = "PAR_CLOSE",
	[7] = "GT",
	[8] = "LT",
	[9] = "SQUOTE",
	[10] = "DQUOTE",
	[11] = "ENV_VAR",
	[12] = "WORD",
	[13] = "OR_OPR",
	[14] = "ARITH_EXPAN"
	};
	ptr = head;

	size = token_size(ptr);
	printf("\n"WHITE_TEXT MAGENTA_BACKGROUND"LIST OF TOKENS"RESET_COLOR"\t[%d]\n", con);
	printf("--------------------\n");
	while (size--)
	{
		if (ptr && ptr->str)
		{
			printf(MAGENTA_TEXT"%pTOKEN [%02d]"RESET_COLOR"\tid: %s [%d]\n\t\tstr: |%s|\tstr address: %p\n",ptr, (con - size), token_name[ptr->id], ptr->id, ptr->str, ptr->str);
			ptr = ptr->next;
		}
	}
	return ;
}

void	print_cmd_args(char **arg)
{
	size_t	i;

	i = 0;
	fprintf(stderr, MAGENTA_TEXT WHITE_BACKGROUND"ARGUMENTS" RESET_COLOR"\n");
	while (arg[i] != NULL)
	{
		fprintf(stderr, "arg [%02zu]:\t|%s| arg adress: %p\n", i, arg[i], &arg[i]);
		i++;
	}
}

void	print_redir(t_redir *redir_list_head)
{
	t_redir	*current_redir;
	char	*redir_name[5] = {
	[0] = "REDIR",
	[1] = "INPUT",
	[2] = "HERE_DOC",
	[3] = "OUTPUT",
	[4] = "APPEND"
	};

	if (redir_list_head == NULL)
	{
		fprintf(stderr, "\n"MAGENTA_TEXT WHITE_BACKGROUND"NO REDIRECTIONS"RESET_COLOR"\n");
		return ;
	}
	current_redir = redir_list_head;
	fprintf(stderr, "\n"MAGENTA_TEXT WHITE_BACKGROUND"REDIRECTIONS"RESET_COLOR"\n");
	while (current_redir != NULL)
	{
		fprintf(stderr, "TYPE [%d]\t%s\n", current_redir->redir_id, redir_name[current_redir->redir_id]);
		fprintf(stderr, "FILE\t\t%s\n", current_redir->file);
		current_redir = current_redir->next;
	}
}

void	print_cmd(t_cmd *cmd)
{
	fprintf(stderr, "\n"WHITE_TEXT MAGENTA_BACKGROUND"CMD"RESET_COLOR"\n");
	fprintf(stderr, "---------------------\n");
	if (cmd)
	{
		print_cmd_args((char **)cmd->args);
		print_redir(cmd->redir);
		fprintf (stderr, "latest_in is %d and latest_out %d\n", cmd->latest_in, cmd->latest_out);
		cmd = cmd->next;
		fprintf(stderr, "______________________\n\n");
	}
}

void print_tree_verbose(t_tree *node, int level)
{
	int i;
	char	*tree_node_name[4] =
	{
		[0] = "AND_OPR",
		[1] = "OR_OPR",
		[2] = "CMD",
		[3] = "PIPE"
	};
	if (!node)
		return;
	i = 0;
	while (i++ < level)
		printf("    ");
	if (node->type == CMD)
		printf(WHITE_TEXT MAGENTA_BACKGROUND"%s"RESET_COLOR" start token:|%s|, end token:|%s|, tree address: %p\n", tree_node_name[node->type], node->start_token->str, node->end_token->str, node);
	else if (node->type == T_AND_OPR || node->type == T_OR_OPR || node->type == T_PIPE)
		printf(MAGENTA_TEXT"%s"RESET_COLOR" %p\n", tree_node_name[node->type], node);
	else
		{
			printf("NULL\n");
			exit(EXIT_FAILURE);
		}
	print_tree_verbose(node->left, level + 1);
	print_tree_verbose(node->right, level + 1);
}


void print_tree(t_tree *node, int level) 
{
	int i;
	char	*tree_node_name[4] = 
	{
		[0] = "AND_OPR",
		[1] = "OR_OPR",
		[2] = "CMD",
		[3] = "PIPE"
	};
	if (!node)
		return;
	i = 0;
	while (i < level)
	{
		fprintf(stderr, "    ");
		i++;
	}
	if (node->type == CMD)
		fprintf(stderr, GREY_BACKGROUND"%s"RESET_COLOR" |%s| |%s|\n", tree_node_name[node->type], node->start_token->str, node->end_token->str);
	else if (node->type == T_AND_OPR || node->type == T_OR_OPR || node->type == T_PIPE)
		fprintf(stderr, MAGENTA_TEXT"%s"RESET_COLOR"\n", tree_node_name[node->type]);
	else
		{
			fprintf(stderr, "NULL\n");
			exit(EXIT_FAILURE);
		}
	print_tree(node->left, level + 1);
	print_tree(node->right, level + 1);
}
