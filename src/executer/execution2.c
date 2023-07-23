/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yraiss <yraiss@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 10:13:06 by ndahib            #+#    #+#             */
/*   Updated: 2023/07/23 18:55:40 by yraiss           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//changes here
void	dup_fd(int *pipe, int fd)
{
	if (fd > -1 && pipe[0] > -1)
		dup2(pipe[0], fd);
	close(pipe[0]);
	pipe[0] = -1;
}

int	execute_compound_cmnd(t_simple_cmd *cmd, t_env **env, int nbr)
{
	int		i;
	pid_t	pid;
	int		pipe_fd[2];
	char	**env_array;
	int		input_fd;
	int		status;

	i = -1;
	pipe_fd[0] = -1;
	pipe_fd[1] = -1;
	input_fd = -1;
	env_array = convert_env_to_array(*env);
	while (++i < nbr)
	{
		if (i != nbr - 1)
			pipe(pipe_fd);
		pid = fork();
		if (pid == -1)
			return (perror("fork :"),1);
		if (pid == 0)
		{
			dup_fd(&pipe_fd[1], STDOUT_FILENO);
			if (i != 0)
				dup_fd(&pipe_fd[0], STDIN_FILENO);
			else
				close(pipe_fd[0]);
			dup_fd(&input_fd, STDIN_FILENO);
			if (redirections(cmd->files) == 1)
				exit(EXIT_FAILURE);
			if (cmd->fd != -1)
			{
				dup2(cmd->fd, 0);
				close(cmd->fd);
			}
			if (is_built_ins(env, cmd) == 0)
				exit(EXIT_SUCCESS);
			else 
			{
				if (execve(cmd->path, cmd->arg, env_array) == -1)
				{
					perror("minishell:\n");
					exit(127);
				}
				exit(EXIT_SUCCESS);	
			}
			exit(EXIT_FAILURE);
		}
		dup_fd(&input_fd, -1);
		if (pipe_fd[0] != -1)
		{
			input_fd = pipe_fd[0];
			pipe_fd[0] = -1;
		}
		dup_fd(&pipe_fd[1], -1);
		dup_fd(&pipe_fd[0], -1);
		cmd = cmd->next;
	}
	for (int i = 0; i < nbr; i++)
	{
		wait(&status);
		if ( WIFEXITED(status) )
			exit_status = WEXITSTATUS(status);
	}
	free_double_pointer(env_array);
	return (exit_status);
}

int	execute_one_simple_cmd(t_simple_cmd *one_cmd, t_env **env)
{
	pid_t	pid;
	int		status;
	char	**env_array;

	if (check_is_built_ins(one_cmd->cmd) == 0)
	{
		if (is_built_ins(env, one_cmd) == 0)
			exit_status = 0;
		else
			exit_status = 1;
		return (exit_status);
	}
	else
	{
		env_array = convert_env_to_array(*env);
		pid = fork();
		if (pid == -1)
			return ((perror("fork"), 1));
		else if (!pid)
		{
			signal(SIGQUIT, SIG_IGN);
			if (redirections(one_cmd->files) == 1)
				return  (EXIT_FAILURE);
			if (one_cmd->fd != -1)
			{
				dup2(one_cmd->fd, 0);
				close(one_cmd->fd);
			}
			// signal(SIGINT, SIG_DFL);
			if (one_cmd->path == NULL)
			{
				printf("minishell: command not found\n");
				exit(127);
			}
			else if (execve(one_cmd->path, one_cmd->arg, env_array) == -1)
			{
				perror("minishell:\n");
				exit(127);
			}
			else
				exit(EXIT_SUCCESS);
		}
		else
		{	
			waitpid(pid, &status, 0);
			if ( WIFEXITED(status) )
				exit_status = WEXITSTATUS(status);
		}
		// signal(SIGINT, SIG_DFL);
		// signal(SIGINT, handle_sig);
		free_double_pointer(env_array);
	}
	return (exit_status);
}

int	execute_commands(t_simple_cmd *cmnd, t_env **env)
{
	int		nbr;
	int		status;

	nbr = get_number_of_cmds(cmnd);
	if (nbr == 1)
		status = execute_one_simple_cmd(cmnd, env);
	else
		status = execute_compound_cmnd(cmnd, env, nbr);
	return (status);
}
