/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbohm <gbohm@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 15:44:18 by gbohm             #+#    #+#             */
/*   Updated: 2023/02/02 16:41:07 by gbohm            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "get_next_line.h"
#include "libft.h"
#include "pipex.h"

#define STDIN 0
#define STDOUT 1

int	throw(int code)
{
	perror(strerror(code));
	return (code);
}

// int	split2(const char *str, char c, char ***result)
// {
// 	*result = ft_split(str, c);
// 	return (*result == NULL);
// }


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
			return (2);
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
			continue ;
		free_arr(paths);
		return (0);
	}
	free(*path);
	return (1);
}

int	main(int argc, const char **argv, char *const *envp)
{
	// int 	fd;
	// char	*stdin;
	// char	**args;

	// if (argc == 1)
	// 	return (throw(22));

	(void) argc;
	(void) argv;
	// (void) envp;

	int fd = open("./src/infile", O_RDONLY);

	printf("open: %d\n", fd);

	// while(*envp)
	// 	printf("%s\n", *envp++);
	printf("=================\n");
	char **params;
	char *path;

	// ft_split2("wc -l", ' ', &params);
	// ft_split2("grep c", ' ', &params);
	// printf("%s\n", path);

	int i = 2;
	while (i < argc - 1)
	{
		printf("%s\n", "test");
		printf("%s\n", argv[i]);
		if (ft_split2(argv[i], ' ', &params))
			return (1);
		if (get_command_path(params[0], envp, &path))
			return (free_arr(params), 2);

		int ports[2];
		if(pipe(ports) == -1)
			return (1);

		pid_t pid = fork();
		printf("process: %d\n", pid);

		if (pid == 0)
		{
			close(ports[0]);
			dup2(fd, STDIN);
			dup2(ports[1], STDOUT);
			close(fd);
			close(ports[1]);
			execve(path, params, envp);
		}
		else
		{
			waitpid(pid, NULL, 0);
			close(ports[1]);
			close(fd);
			fd = ports[0];
		}
		free(path);
		free_arr(params);
		i++;
	}
	printf("%s\n", get_next_line(fd));
	close(fd);

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
	// 	// printf("'%s'\n", line);
	// 	// printf("read %d bytes: '%s'\n", bytes, buf);
	// }


	// printf("stdin: '%s'\n", stdin);

	// fd = open("infile", O_RDONLY);

	return (0);
}
