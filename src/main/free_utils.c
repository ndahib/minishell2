/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yraiss <yraiss@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 08:07:55 by ndahib            #+#    #+#             */
/*   Updated: 2023/07/25 01:42:09 by yraiss           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_double_pointer(char **pointer)
{
	int	i;

	i = 0;
	if (pointer == NULL)
		return ;
	while (pointer[i] != NULL)
	{
		free(pointer[i]);
		pointer[i] = NULL;
		i++;
	}
	free (pointer);
}

void	free_files(t_files **files)
{
	t_files	*tmp;

	tmp = *files;
	if (files == NULL || *files == NULL)
		return ;
	while (tmp != NULL)
	{
		free(tmp->file);
		tmp = tmp->next;
	}
	free(*files);
}

void	free_lst_of_cmd(t_simple_cmd **head, int trigger)
{
	t_simple_cmd	*tmp;
	t_simple_cmd	*tmp2;

	tmp = *head;
	while (tmp != NULL)
	{
		if (trigger != 127)
			free(tmp->cmd); 
		free(tmp->path);
		free_double_pointer(tmp->arg);
		free_files(&tmp->files);
		tmp2 = tmp;
		tmp = tmp->next;
		free(tmp2);
		tmp = NULL;
		// system("leaks minishell");
	}
}
