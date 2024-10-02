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
	printf("LINKED LISTOF ENV VARS:\n\n");
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
	char	*token_name[16] = {
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
	[12] = "ENV_VAR",
	[13] = "WORD",
	[14] = "OR_OPR",
	[15] = "ARITH_EXPAN"
	};
	ptr = head;

	size = token_size(ptr);
	printf("\n"WHITE_TEXT MAGENTA_BACKGROUND"LIST OF TOKENS"RESET_COLOR"\t[%d]\n", con);
	printf("--------------------\n");
	while (size--)
	{
		if (ptr && ptr->str)
		{
			printf(MAGENTA_TEXT"%pTOKEN [%02d]"RESET_COLOR"\tid: %s [%d]\n\t\tstr: |%s|\n",ptr, (con - size), token_name[ptr->id], ptr->id, ptr->str);
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
		fprintf(stderr, "arg [%02zu]:\t|%s|\n", i, arg[i]);
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

void	print_cmd(t_cmd *cmd_list_head)
{
	int			size;
	t_cmd		*current_cmd;
	int	con = cmd_size(cmd_list_head);

	size = cmd_size(cmd_list_head);
	fprintf(stderr, "\n"WHITE_TEXT MAGENTA_BACKGROUND"LIST OF CMDS"RESET_COLOR"\t [%02d]\n", size);
	fprintf(stderr, "---------------------\n");
	current_cmd = cmd_list_head;
	while (current_cmd != NULL)
	{
		fprintf(stderr, MAGENTA_TEXT"CMD [%d]"RESET_COLOR"\n\n", (con - --size));
		print_cmd_args((char **)current_cmd->args);
		print_redir(current_cmd->redir);
		fprintf (stderr, "latest_in is %d and latest_out %d\n", current_cmd->latest_in, current_cmd->latest_out);
		current_cmd = current_cmd->next;
		fprintf(stderr, "______________________\n\n");
	}
}

