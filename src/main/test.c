/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yraiss <yraiss@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 15:42:32 by ndahib            #+#    #+#             */
/*   Updated: 2023/07/23 18:54:44 by yraiss           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    handle_sig(int sig)
{
	if (sig == SIGINT)
	{
		printf("im here\n");
		exit_status = 130;
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("",0);
		rl_redisplay();
	}
}

int	minishell_loop(t_env **env_lst)
{
	char			**tokens;
	char			*cmd_line;
	t_simple_cmd	*parse_cmd;

	signal(SIGQUIT, SIG_IGN);
	// signal(SIGINT, handle_sig);
	while (1)
	{
		cmd_line = readline(BLUE"❄️ minishell ➡️ "GREEN);
		if (!cmd_line)
			exit(exit_status);
		if (!cmd_line)
			break ;
		if (!ft_strlen(cmd_line))
			continue ;
		else
			add_history(cmd_line);
		tokens = tokeniser(cmd_line);
		if (check_syntax_err(tokens) == 1)
		{
			free_double_pointer(tokens);
			free(cmd_line);
			exit_status = 0;
			continue ;
		}
		parse_cmd = parse_simple_cmd(*env_lst, tokens);
		if (execute_commands(parse_cmd, env_lst) == 0)
			continue;
			// printf("succed in execution\n");
		// free_lst_of_cmd(&parse_cmd);
		free_double_pointer(tokens);
		free(cmd_line);
	}
	return (1);
}

int	main(int ac, char **av, char **env)
{
	t_env			*env_lst;

	(void)ac;
	(void)av;
	env_lst = handle_env(env);
	minishell_loop(&env_lst);
	// free_lst(env_lst);
	while (1);
	return (0);
}
