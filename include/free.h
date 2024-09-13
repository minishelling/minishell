/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   free.h                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/17 15:44:17 by lprieri       #+#    #+#                 */
/*   Updated: 2024/07/25 18:02:32 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef FREE_H
# define FREE_H

# include "minishell.h"

void	ft_free(void **var);
void	ft_free_2d(void ***arr);
void	ft_free_3d(void ****arr);

#endif