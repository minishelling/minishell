/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/24 13:58:20 by lprieri       #+#    #+#                 */
/*   Updated: 2024/06/25 14:35:44 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

typedef struct s_token
{
	t_token_type	type;
	
}	t_token;

typedef enum e_metacharacter
{
	METACHAR_SPACE,
	METACHAR_TAB,
	METACHAR_NEWLINE,
	METACHAR_PIPE,
	METACHAR_AND,
	METACHAR_SEMICOLON,
	METACHAR_OPEN_PAREN,
	METACHAR_CLOSE_PAREN,
	METACHAR_GREATER,
	METACHAR_LESSER,
	METACHAR_COUNT
}	t_metacharacter;

typedef enum e_control_operator
{
	CO_NEWLINE,
	CO_LOGICAL_OR,
	CO_LOGICAL_AND,
	CO_BACKGROUND_EXECUTION,
	CO_COMMAND_SEPARATOR,
	CO_CASE_TERMINATOR,
	CO_FALLTHROUGH_CASE_TERMINATOR,
	CO_CASE_TERMINATOR_WITH_FALLTHROUGH,
	CO_PIPE,
	CO_PIPE_WITH_STANDARD_ERROR,
	CO_SUBSHELL,
	CO_COUNT
}	t_control_operator;


#endif