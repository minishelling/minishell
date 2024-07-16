#include "../include/minishell.h"

void add_token(t_shell *shell, e_token_type type, e_token_id id, char *str) {
    t_token *new_token = (t_token *)malloc(sizeof(t_token));
    new_token->id = id;
    new_token->type = type;
    new_token->str = strdup(str);
    new_token->next = NULL;

    if (!shell->token) {
        shell->token = new_token;
    } else {
        t_token *temp = shell->token;
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = new_token;
    }
}

void tokenize_input(t_shell *shell)
{
    char *input = shell->input;
    while (*input)
	{
        if (*input == ' ') 
		{
            add_token(shell, META_CHAR, META_SPACE, " ");
            input++;
            // Skip consecutive spaces
            while (*input && *input == ' ') 
			{
                input++;
            }
        } 
		else if (*input == '\t')
		{
            add_token(shell, META_CHAR, META_TAB, "\t");
            input++;
        } 
		else if (*input == '\n')
		{
            add_token(shell, CONTROL_OP, CTRL_CMD_TERM, "\n");
            input++;
        }
		else if (strncmp(input, "&&", 2) == 0) 
		{
            add_token(shell, CONTROL_OP, CTRL_LOG_AND, "&&");
            input += 2;
        } 
		else if (strncmp(input, "||", 2) == 0) 
		{
            add_token(shell, CONTROL_OP, CTRL_LOG_OR, "||");
            input += 2;
        }
		else if (strncmp(input, ";;", 2) == 0) 
		{
            add_token(shell, CONTROL_OP, CTRL_DOUBLE_SEMI, ";;");
            input += 2;
        } 
		else if (strncmp(input, ";&", 2) == 0) 
		{
            add_token(shell, CONTROL_OP, CTRL_CASE_FALL, ";&");
            input += 2;
        }
		else if (strncmp(input, ";;&", 3) == 0) {
            add_token(shell, CONTROL_OP, CTRL_DOUBLE_CASE_FALL, ";;&");
            input += 3;
        }
		else if (strncmp(input, "|&", 2) == 0) {
            add_token(shell, CONTROL_OP, CTRL_PIPE_AND, "|&");
            input += 2;
        }
		else if (*input == '|')
		{
            add_token(shell, META_CHAR, META_PIPE, "|");
            input++;
        }
		else if (*input == '&')
		{
            add_token(shell, META_CHAR, META_AND_OPR, "&");
            input++;
        }
		else if (*input == ';')
		{
            add_token(shell, CONTROL_OP, CTRL_SEMI, ";");
            input++;
        } 
		else if (*input == '{')
		{
            add_token(shell, CONTROL_OP, CTRL_PAR_OPEN, "{");
            input++;
        } 
		else if (*input == '}') 
		{
            add_token(shell, CONTROL_OP, CTRL_PAR_CLOSE, "}");
            input++;
        } 
		else if (strncmp(input, ">>", 2) == 0) {
            add_token(shell, REDIR_OP, REDIR_OUT_APPEND, ">>");
            input += 2;
        } 
		else if (strncmp(input, "<<", 2) == 0) {
            add_token(shell, REDIR_OP, REDIR_IN_HEREDOC, "<<");
            input += 2;
        } 
		else if (strncmp(input, "&>", 2) == 0) {
            add_token(shell, REDIR_OP, REDIR_OUT_ERR, "&>");
            input += 2;
        } 
		else if (strncmp(input, "<>", 2) == 0) {
            add_token(shell, REDIR_OP, REDIR_IN_OUT, "<>");
            input += 2;
        } 
		else if (strncmp(input, "&>>", 3) == 0) {
            add_token(shell, REDIR_OP, REDIR_OUT_ERR_APPEND, "&>>");
            input += 3;
        } 
		else if (strncmp(input, ">&", 2) == 0) {
            add_token(shell, REDIR_OP, REDIR_OUT_FD, ">&");
            input += 2;
        } 
		else if (strncmp(input, "<<<", 3) == 0) {
            add_token(shell, REDIR_OP, REDIR_IN_OUT_HEREDOC, "<<<");
            input += 3;
        } 
		else if (strncmp(input, "<&", 2) == 0) {
            add_token(shell, REDIR_OP, REDIR_IN_FD, "<&");
            input += 2;
        } 
		else 
		{
            // Handle generic words
            char *start = input;
            while (*input && *input != ' ' && *input != '\t' && *input != '\n' &&
                   *input != '|' && *input != '&' && *input != ';' &&
                   *input != '{' && *input != '}' && *input != '<' && *input != '>' &&
                   *input != '$' && *input != '?') 
				   {
                input++;
            }
            char *word = strndup(start, input - start);
            add_token(shell, WORD, 0, word);
            free(word);
        }
    }
}

void print_tokens(t_token *token) 
{
    const char *token_type_names[] = 
	{
        "META_CHAR",
        "CONTROL_OP",
        "REDIR_OP",
        "WORD",
        "OTHER"
    };

    while (token) 
	{
        printf("Token: |%s|, Type: %s, ID: %d\n", token->str, token_type_names[token->type], token->id);
        token = token->next;
    }
}
