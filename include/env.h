/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env.h                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/09 20:42:57 by lprieri       #+#    #+#                 */
/*   Updated: 2024/07/09 20:42:57 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include "minishell.h"

typedef struct s_env
{
	char			*keyvalue;
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef enum
{
	ENV_SUCCESS = 0,
	ENV_NULL,
	ENV_NEW_NODE_ERROR,
	ENV_GETKEYVALUE_ERROR,
	ENV_GETKEY_ERROR,
	ENV_GETVALUE_ERROR,
	ENV_COUNT
}	t_env_ecode;

/**
* @brief env_free_list frees the 3 strings: 'keyvalue', 'key' and 'value' 
*		 in each node of the env list.
*
* @param head Pointer to the first node of env list.
*/
void	env_free_list(t_env	**head);

/**
* @brief env_init_list takes a pointer to the list and populates it with
*		 nodes, one for each key-value pair. Each node contains 3 strings:
*		 'keyvalue', 'key' and 'value'.
*
* @param head Pointer to the first node of env list.
* @param envp The environment pointer that contains the initial key-value pairs.
* @return A code(int) indicating either success or error.	
*/
t_env_ecode	env_init_list(t_env **head, char **envp);

/**
* @brief Prints the contents of each node of the environment list,
*		 with a newline character separating each node's output.
*
* @param head Pointer to the first node of env list.
*/
void	env_print_list(t_env *head);

/**
* @brief Counts how many key-value pairs there are in the environment pointer.
*
* @param envp The environment pointer.
* @return Returns the number of key-value pairs there are in envp,
*		  or -1 if envp was NULL.
*/
ssize_t	env_count_keys(char **envp);

/**
* @brief Mallocs a new node and initializes each variable to NULL.
*
* @param void
* @return The new node.
*/
t_env	*env_new_node(void);

/**
* @brief Retrieves the key substring from a key-value string.
*
* @param keyvalue The current keyvalue string from envp.
* @return Returns the new key string.
*/
char	*env_get_key(char *keyvalue);

/**
* @brief Retrieves the value substring from a key-value string.
*
* @param keyvalue The current keyvalue string from envp.
* @return Returns the new value string.
*/
char	*env_get_value(char *keyvalue);

/**
* @brief Copies the keyvalue string from envp into a new node,
*		 and copies the key and value substrings into new strings.
*
* @param new_node Pointer to a new node that will receive the copied values.
* @param keyvalue The current keyvalue string from envp.
* @return Returns the new node with the copied values.
*/
t_env_ecode	env_copy_keyval(t_env **new_node, char *keyvalue);

/**
* @brief Finds the environment node that corresponds to the variable being searched.
*
* @param env This is the head node of the linked list containing all the environment variables.
* @param key This is the name of the environment variable.
* @return Returns the node that corresponds to that variable.
*/
t_env	*env_find_node(t_env *env, char *key);

/**
*
*/
size_t	env_count_values(t_env *env, char *key);


#endif