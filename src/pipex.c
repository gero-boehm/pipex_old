/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbohm <gbohm@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 15:44:18 by gbohm             #+#    #+#             */
/*   Updated: 2023/02/04 22:08:49 by gbohm            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "get_next_line.h"
#include "libft.h"
#include "pipex.h"

#define STDIN 0
#define STDOUT 1

int get_stdin2(char **str)
{
	size_t	max;
	size_t	len;
	int		read_res;
	char	c;

	*str = ft_strdup("");
	len = 0;
	max = 1;
	while (1)
	{
		read_res = read(0, &c, 1);
		if (read_res == 0)
			return (0);
		if (read_res == -1)
			return (1);
		(*str)[len] = c;
		len++;
		if (len == max)
		{
			max *= 2;
			if (ft_extstr2(str, max))
				return (2);
		}
	}
	return (0);
}

int	starts_with(const char *str, const char *match)
{
	while (*match)
	{
		if (*str++ != *match++)
			return (0);
	}
	return (1);
}

int	ft_substr2(char const *s, unsigned int start, size_t len, char **sub)
{
	*sub = ft_substr(s, start, len);
	return (*sub == NULL);
}

int	ft_strjoin2(char **dst, const char *src)
{
	char	*new;

	if (*dst == NULL)
	{
		*dst = src;
		return (0);
	}
	new = ft_strjoin(*dst, src);
	free(*dst);
	free(src);
	*dst = new;
	return (new == NULL);
}

int	get_paths(char *const *envp, char ***paths)
{
	char	*str;

	while (*envp)
	{
		if(!starts_with(*envp, "PATH="))
		{
			envp++;
			continue ;
		}
		if (ft_substr2(*envp, 5, ft_strlen(*envp) - 5, &str))
			return (1);
		if (ft_split2(str, ':', paths))
			return (free(str), 2);
		free(str);
		break ;
	}
	return (0);
}

void free_arr(void **arr)
{
	void	**cursor;

	cursor = arr;
	while (*cursor != NULL)
		free(*cursor++);
	free(arr);
}

int	join_path2(const char *a, const char *b, const char *c, char **path)
{
	char	*tmp;

	tmp = ft_strjoin(a, b);
	if (tmp == NULL)
		return (1);
	*path = ft_strjoin(tmp, c);
	if (tmp == NULL)
		return (free(tmp), 2);
	free(tmp);
	return (0);
}

int	get_command_path(const char *cmd, char *const *envp, char **path)
{
	char	**paths;
	char	**cursor;

	if (get_paths(envp, &paths))
		return (1);
	cursor = paths;
	while (*cursor)
	{
		if (join_path2(*cursor, "/", cmd, path))
			return (free_arr((void **) paths), 1);
		cursor++;
		if (access(*path, F_OK))
		{
			free(*path);
			continue ;
		}
		free_arr(paths);
		return (0);
	}
	free_arr(paths);
	free(*path);
	return (1);
}

int	run_child(char *cmd, char *const *envp, int fd, int ports[2])
{
	char	**params;
	char	*path;

	close(ports[0]);
	dup2(fd, STDIN);
	dup2(ports[1], STDOUT);
	close(fd);
	close(ports[1]);
	if (ft_split2(cmd, ' ', &params))
		return (1);
	if (get_command_path(params[0], envp, &path))
		return (free_arr(params), 2);
	execve(path, params, envp);
	free(path);
	free_arr(params);
	return (3);
}

int run_parent(pid_t pid, int *fd, int ports[2])
{
	waitpid(pid, NULL, 0);
	close(ports[1]);
	close(*fd);
	*fd = ports[0];
}

int	get_write_fd2(const char *fspath, int append, int *fd)
{
	int	flags;

	flags = O_WRONLY | O_CREAT | O_APPEND;
	if (!append)
		flags |= O_TRUNC;
	*fd = open(fspath, flags, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	return (*fd == -1);
}

int output(const char *file, int fd, int is_here_doc)
{
	int		outfile;
	char	*line;
	size_t	len;

	if (get_write_fd2(file, is_here_doc, &outfile))
		return (1);
	while (1)
	{
		line = get_next_line(fd);
		if (line == NULL)
			break ;
		len = ft_strlen(line);
		write(outfile, line, len);
		free(line);
	}
	close(outfile);
	return (0);
}

int	run(int fd, int is_here_doc, const char **argv, char *const *envp)
{
	int		i;
	int		ports[2];
	pid_t	pid;
	int		argc;

	argc = 0;
	while (argv[argc] != NULL)
		argc++;
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
			run_parent(pid, &fd, ports);
		i++;
	}
	output(argv[argc - 1], fd, is_here_doc);
	close(fd);
}

int	get_here_doc(char *eof)
{
	char	*line;
	int		fd;

	if (get_write_fd2("here_doc", 0, &fd))
		return (1);
	while (1)
	{
		write(1, "heredoc> ", 9);
		line = get_next_line(STDIN);
		if (line == NULL || ft_strncmp(line, eof, ft_strlen(eof)) == 0)
			break ;
		write(fd, line, ft_strlen(line));
		free(line);
	}
	free(line);
	close(fd);
	return (0);
}

int	get_infile2(const char *fspath, int *fd)
{
	*fd = open(fspath, O_RDONLY);
	return (*fd == -1);
}

int	main(int argc, const char **argv, char *const *envp)
{
	int	is_here_doc;
	int	infile;

	if (argc < 3)
		return (1);
	is_here_doc = ft_strncmp(argv[1], "here_doc", 8) == 0;
	if (TWO_COMMANDS_ONLY)
	{
		if (argc != 5)
			return (2);
	}
	else if (is_here_doc)
	{
		if (argc < 5)
			return (3);
		if (get_here_doc(argv[2]))
			return (4);
	}
	if (get_infile2(argv[1], &infile))
		return (5);
	run(infile, is_here_doc, argv, envp);

	// fd = open(argv[1], O_RDONLY);

	// char **params;
	// char *path;

	// int i = 2;
	// while (i < argc - 1)
	// {
	// 	if (ft_split2(argv[i], ' ', &params))
	// 		return (1);
	// 	if (get_command_path(params[0], envp, &path))
	// 		return (free_arr(params), 2);

	// 	int ports[2];
	// 	if(pipe(ports) == -1)
	// 		return (1);

	// 	pid_t pid = fork();
	// 	if (pid == 0)
	// 	{
	// 		close(ports[0]);
	// 		dup2(fd, STDIN);
	// 		dup2(ports[1], STDOUT);
	// 		close(fd);
	// 		close(ports[1]);
	// 		execve(path, params, envp);
	// 	}
	// 	else
	// 	{
	// 		waitpid(pid, NULL, 0);
	// 		close(ports[1]);
	// 		close(fd);
	// 		fd = ports[0];
	// 	}
	// 	free(path);
	// 	free_arr(params);
	// 	i++;
	// }

	// int outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC | O_APPEND);

	// char *res;
	// while ((res = get_next_line(fd)))
	// {
	// 	int len = ft_strlen(res);
	// 	write(outfile, res, len);
	// 	free(res);
	// }
	// close(outfile);
	// close(fd);

	// get_stdin2(&stdin);

	// const char	*limiter = "EOF";
	// size_t len = ft_strlen(limiter);

	// printf("before\n");
	// while(1)
	// {
	// 	// char buf[21] = {0};
	// 	write(1, "heredoc> ", 9);
	// 	char *line = get_next_line(0);
	// 	(void) line;
	// 	printf("'%s'\n", line);
	// 	// printf("read %d bytes: '%s'\n", bytes, buf);
	// }

	// if (get_here_doc("eof"))
	// {
	// 	printf("heredoc failed");
	// 	return (1);
	// }
	// printf("yay\n");
	// int fd = open("here_doc", O_RDONLY);
	// char *res;
	// while ((res = get_next_line(fd)))
	// {
	// 	printf("'%s'\n", res);
	// 	free(res);
	// }
	// close(fd);
	// unlink("here_doc");


	// printf("stdin: '%s'\n", stdin);

	// fd = open("infile", O_RDONLY);
	// system("leaks pipex");
	return (0);
}

