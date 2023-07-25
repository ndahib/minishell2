/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   creat_childs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yraiss <yraiss@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 20:15:45 by ndahib            #+#    #+#             */
/*   Updated: 2023/07/25 01:36:55 by yraiss           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	copy_input_to_fd(int *input_fd, int *pipe_fd)
{
	if (pipe_fd[0] != -1)
	{
		*input_fd = pipe_fd[0];
		pipe_fd[0] = -1;
	}
}

//***************create_child_forone_cmd***************************************
void	create_child(t_simple_cmd *one_cmd, char **env)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		perror("fork :");
	if (pid == 0)
	{
		if (one_cmd->fd != -1)
			dup_close(one_cmd->fd);
		if (check_redir(one_cmd->files) == 1)
			exit(EXIT_FAILURE);
		if (!one_cmd->path)
		{
			printf("minishell: command not found\n");
			exit(127);
		}
		else if (execve(one_cmd->path, one_cmd->arg, env) == -1)
		{
			perror("minishell: command not found\n");
			exit(127);
		}
		exit(EXIT_SUCCESS);
	}
	wait_childs(1);
}

void	execute_cmd(t_simple_cmd *cmd, char **env_array)
{
	int		on;

	on = 0;
	if (cmd->path == NULL)
	{
		printf("minishell : command not found\n");
		free_lst_of_cmd(&cmd, on);
		free_double_pointer(env_array);
		exit(127);
	}
	if (execve(cmd->path, cmd->arg, env_array) == -1)
	{
		perror("minishell:\n");
		free_lst_of_cmd(&cmd, on);
		free_double_pointer(env_array);
		exit(127);
	}
	exit(EXIT_SUCCESS);
}

//***************create_child_for_multiples_cmds*******************************
void	create_childs(t_simple_cmd *cmd, t_env **env, t_pipe_files *var)
{
	pid_t		pid;
	char		**env_arr;

	pid = fork();
	if (pid == -1)
		perror("fork :");
	env_arr = NULL;
	if (pid == 0)
	{
		env_arr = convert_env_to_array(*env);
		dup_fd(&(var->pipe_fd[1]), STDOUT_FILENO);
		if (var->i != 0)
			dup_fd(&(var->pipe_fd[0]), STDIN_FILENO);
		else
			close(var->pipe_fd[0]);
		dup_fd(&(var->input_fd), STDIN_FILENO);
		if (check_redir(cmd->files) == 1)
			exit(EXIT_FAILURE);
		if (cmd->fd != -1)
			dup_close(cmd->fd);
		if (is_built_ins(env, cmd) == 0)
			exit(EXIT_SUCCESS);
		else
			execute_cmd(cmd, env_arr);
		exit(1);
	}
}

int	run_built_ins(t_env **env, t_simple_cmd *cmd)
{
	if (is_built_ins(env, cmd) == 0)
	{
		g_exit_status = 0;
		return (0);
	}
	else
	{
		g_exit_status = 1;
		return (1);
	}
}
