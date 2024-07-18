#include "../../include/minishell.h"

typedef bool	(*t_syntax_func) (t_token *t_prev, t_token *t_cur, \
		t_env *env_list);

t_token	*list_token_start_nonspace(t_token *list)
{
	if (list == NULL)
		return (NULL);
	if (list->id == SPACE_CHAR || list->id == TAB_CHAR || list->id == NL)
		return (list->next);
	return (list);
}

t_token	*syntax(t_token *top, t_env *env_list)
{
	t_token				*t_prev;
	t_token				*t_cur;
	const t_syntax_func	func[14] = {
	[0] = NULL,
	[1] = NULL,
	[2] = NULL,
	[3] = syntax_id_pipe,
	[4] = syntax_id_ampersand,
	[5] = syntax_id_semicol,
	[6] = syntax_id_parentheses_open,
	[7] = syntax_id_parentheses_close,
	[8] = syntax_id_redir,
	[9] = syntax_id_redir,
	[10] = syntax_id_misc,
	[11] = syntax_id_misc,
	[12] = syntax_id_misc,
	[13] = syntax_id_misc,
	};

	t_prev = NULL;
	t_cur = list_token_start_nonspace(top);
	while (t_cur != NULL)
	{
		printf ("in syntax, t_cur->id is %d\n", t_cur->id);
		if (func[t_cur->id](t_prev, t_cur, env_list))
			return (t_cur);
		t_prev = t_cur;
		t_cur = list_token_skip_space(t_cur);
	}
	
	return (NULL);
}

// if only space tokens are given wat to do?

/* -------------------------------------------------------
   The grammar symbols
   ------------------------------------------------------- 

%token  WORD

%token   DLESS    DGREAT
         '<<'     '>>'    

   -------------------------------------------------------
   The Grammar
   ------------------------------------------------------- 
   
https://stackoverflow.com/questions/76167393/context-free-grammar-clarification

%start  pipe_sequence
%%
pipe_sequence    :                   command
                 | pipe_sequence '|' command
                 ;
command          : cmd_prefix cmd_name cmd_suffix
                 | cmd_prefix cmd_name
                 | cmd_prefix
                 | cmd_name cmd_suffix
                 | cmd_name
                 ;
cmd_name         : WORD                     Apply rule 1
                 ;
cmd_prefix       :            io_redirect
                 | cmd_prefix io_redirect
                 ;
cmd_suffix       :            io_redirect
                 | cmd_suffix io_redirect
                 |            WORD
                 | cmd_suffix WORD
                 ;
io_redirect      : '<'       filename
                 | '>'       filename
                 | DGREAT    filename
                 |           io_here
                 ;
filename         : WORD                     Apply rule 2
                 ;
io_here          : DLESS     here_end
                 ;
here_end         : WORD                     Apply rule 3
                 ;

   --------------------------------------------------------- */
