/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbohm <gbohm@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 14:12:05 by gbohm             #+#    #+#             */
/*   Updated: 2023/03/14 17:59:36 by gbohm            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "libft.h"
#include "pipex.h"

static int	get_argc(const char **argv)
{
	int	count;

	count = 0;
	while (argv[count] != NULL)
		count++;
	return (count);
}

static void	run_child(const char *cmd, char *const *envp, int fd, int ports[2])
{
	char	**params;
	char	*path;

	close(ports[0]);
	dup2(fd, STDIN);
	dup2(ports[1], STDOUT);
	close(fd);
	close(ports[1]);
	if (split_params2(cmd, &params))
		exit(1);
	if (get_command_path2(params[0], envp, &path))
	{
		ft_putstr_fd("pipex: ", 2);
		ft_putstr_fd(params[0], 2);
		ft_putendl_fd(": command not found", 2);
		ft_arrfree((void **) params);
		exit(2);
	}
	execve(path, params, envp);
	ft_putendl_fd("pipex: failed to execute command", 2);
	free(path);
	ft_arrfree((void **) params);
	exit(3);
}

static void	run_parent(int *fd, int ports[2])
{
	close(ports[1]);
	close(*fd);
	*fd = ports[0];
}

int	run(int fd, int is_here_doc, const char **argv, char *const *envp)
{
	int		i;
	int		ports[2];
	pid_t	pid;
	int		argc;

	argc = get_argc(argv);
	i = is_here_doc + 2;
	while (i < argc - 1)
	{
		if (pipe(ports) == -1)
			return (1);
		pid = fork();
		if (pid == -1)
			return (2);
		if (pid == 0)
			run_child(argv[i], envp, fd, ports);
		else
			run_parent(&fd, ports);
		i++;
	}
	while (i-- - is_here_doc - 2)
		waitpid(0, NULL, 0);
	if (output(argv[argc - 1], fd, is_here_doc))
		return (ft_putendl_fd("pipex: error writing to outfile", 2), 2);
	close(fd);
	return (0);
}
