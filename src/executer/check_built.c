/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_built.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yraiss <yraiss@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 19:37:37 by ndahib            #+#    #+#             */
/*   Updated: 2023/07/25 02:00:00 by yraiss           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_is_built_ins(char *cmd)
{
	if (ft_strncmp(cmd, "echo", 5) == 0)
		return (0);
	else if (ft_strncmp(cmd, "cd", 3) == 0)
		return (0);
	else if (ft_strncmp(cmd, "env", 4) == 0)
		return (0);
	else if (ft_strncmp(cmd, "pwd", 4) == 0)
		return (0);
	else if (ft_strncmp(cmd, "unset", 6) == 0)
		return (0);
	else if (ft_strncmp(cmd, "export", 7) == 0)
		return (0);
	else if (ft_strncmp(cmd, "exit", 5) == 0)
		return (0);
	return (1);
}

void	dup_and_close(int *std_fd)
{
	dup2(std_fd[0], STDIN_FILENO);
	dup2(std_fd[1], STDOUT_FILENO);
	close(std_fd[1]);
	close(std_fd[0]);
}

int	other_built(t_env **lst, t_simple_cmd *cmd)
{
	if (ft_strncmp(cmd->cmd, "echo", 5) == 0)
		my_echo(cmd->arg);
	else if (ft_strncmp(cmd->cmd, "cd", 3) == 0)
	{
		if (my_cd(cmd, lst))
			return (1);
	}
	else if (ft_strncmp(cmd->cmd, "env", 4) == 0)
		my_env(lst);
	else if (ft_strncmp(cmd->cmd, "pwd", 4) == 0)
		my_pwd();
	return (0);
}

int	builtins_exec(t_env ***lst, t_simple_cmd *cmd)
{
	int	i;

	i = 1;
	if (other_built(*lst, cmd))
		return (1);
	else if (ft_strncmp(cmd->cmd, "unset", 6) == 0)
	{
		while ((cmd->arg[i]) != NULL)
		{
			unset_variable(lst, cmd->arg[i]);
			i++;
		}
	}
	else if (ft_strncmp(cmd->cmd, "export", 7) == 0)
	{
		if (my_export(lst, cmd->arg + 1))
			return (1);
	}
	else if (ft_strncmp(cmd->cmd, "exit", 5) == 0)
		my_exit(cmd->arg);
	return (0);
}

int	is_built_ins(t_env **lst, t_simple_cmd *cmd)
{
	int	i;
	int	std_fd[2];

	i = 1;
	if (cmd->cmd == NULL)
		return (1);
	std_fd[0] = dup(STDIN_FILENO);
	std_fd[1] = dup(STDOUT_FILENO);
	if (redirections(cmd->files) == 1)
		return (1);
	if (!builtins_exec(&lst, cmd))
		return (0);
	else
	{
		dup_and_close(std_fd);
		return (1);
	}
	dup_and_close(std_fd);
	return (0);
}
