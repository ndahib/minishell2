/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yraiss <yraiss@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 15:42:32 by ndahib            #+#    #+#             */
/*   Updated: 2023/07/25 01:13:38 by yraiss           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    handle_sig(int sig)
{
	if (sig == SIGINT)
	{
		g_exit_status = 130;
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("",0);
		rl_redisplay();
	}
}

void	free_all(t_simple_cmd **lst, char **tokens, char *line, int trigger)
{
	free_lst_of_cmd(lst, trigger);
	free_double_pointer(tokens);
	free(line);
}

void	minishell_loop(t_env **env_lst)
{
	char			**tokens;
	char			*cmd_line;
	t_simple_cmd	*parse_cmd;

	(void)env_lst;
	parse_cmd = NULL;
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, handle_sig);
	while (1)
	{
		cmd_line = readline("minishell-> "CYAN);
		if (!cmd_line)
			exit(g_exit_status);
		if (!ft_strlen(cmd_line))
		{
			free(cmd_line);
			continue ;
		}
		else
			add_history(cmd_line);
		tokens = tokeniser(cmd_line);
		if (check_syntax_err(tokens) == 1)
		{
			free_double_pointer(tokens);
			free(cmd_line);
			g_exit_status = 2;
			continue ;
		}
		parse_cmd = parse_simple_cmd(*env_lst, tokens);
		int trigger = execute_commands(parse_cmd, env_lst);
		free_all(&parse_cmd, tokens, cmd_line, trigger);
	}
}

int	main(int ac, char **av, char **env)
{
	t_env	*env_lst;

	(void)ac;
	(void)av;
	env_lst = handle_env(env);
	minishell_loop(&env_lst);
	return (0);
}

		// while (parse_cmd)
		// {
		// 	printf("%d\n", parse_cmd->fd);
		// 	printf("%s\n", parse_cmd->cmd);
		// 	printf("%s\n", parse_cmd->path);
		// 	printf_double_pointer(parse_cmd->arg);
		// 	print_lst(parse_cmd->files);
		// 	parse_cmd = parse_cmd->next;
		// }
