/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yraiss <yraiss@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 20:28:31 by yraiss            #+#    #+#             */
/*   Updated: 2023/07/23 18:56:03 by yraiss           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//changes here
char	*get_buffer(char *del, t_env *env)
{
	char	*line;
	char	*final;
	char	*new_del;
	char	*new_final;
	int		check;

	final = "";
	line = NULL;
	new_del = NULL;
	new_final = NULL;
	check = check_del(del);
	if (check == 1)
		new_del = remove_quotes(del);
	else
		new_del = del;
	while (1)
	{
		ft_putstr_fd("> ", 1);
		line = get_next_line(0);
		if (ft_strncmp(line, new_del, ft_strlen(line) - 1) == 0)
		{
			free(line);
			break ;
		}
		final = ft_strjoin(final, line);
		free(line);
		line = NULL;
	}
	if (check == 0)
	{
		char *tmp;
		tmp = ft_strdup(final);
		new_final = expand_her_doc(final, env);
		free(tmp);
		return (new_final);
	}
	free(del);
	free(new_del);
	return (final);
}

int	her_doc(char *del, t_env *env)
{
	int		fd[2];
	char	*buff;
	pid_t	pid;

	buff = "";
	if (pipe(fd) == -1)
		perror("pipe : ");
	pid = fork();
	if (pid == -1)
		return (perror("fork in herdoc : \n"), -1);
	else if (!pid)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_IGN);
		close(fd[0]);
		buff = get_buffer(del, env);
		ft_putstr_fd(buff, fd[1]);
		if (ft_strlen(buff) != 0)
			free(buff);
		close(fd[1]);
		exit(EXIT_SUCCESS);
		// signal(SIGINT, SIG_DFL);
	}
	close(fd[1]);
	wait(NULL);
	return (fd[0]);
}

// int	check_and_dup(fd, duplicated_file)
// {
// 	if (fd == -1)
// 		return (1);
// 	dup2(fd, duplicated_file);
// 	close (fd);
// 	return (0);
// }

int	redirections(t_files *files)
{
	int		fd;
	t_files	*tmp;

	tmp = files;
	while (tmp)
	{
		if (tmp->type == 3)
		{
			fd = open(tmp->file, O_RDWR | O_CREAT | O_TRUNC,  0644);
			if (fd == -1)
				return (perror("OPEN "), 1);
			dup2(fd, STDOUT_FILENO);
			close (fd);
		}
		else if (tmp->type == 4)
		{
			fd = open(tmp->file, O_RDONLY);
			if (fd == -1)
				return (perror("OPEN "), 1);
			dup2(fd, 0);
			close (fd);
		}
		else if (tmp->type == 5)
		{
			fd = open(tmp->file, O_RDWR | O_CREAT | O_APPEND, 0777);
			if (fd == -1)
				return (perror("OPEN "), 1);
			dup2(fd, 1);
			close (fd);
		}
		tmp = tmp->next;
	}
	return (0);
}