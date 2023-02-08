/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbohm <gbohm@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 15:44:18 by gbohm             #+#    #+#             */
/*   Updated: 2023/02/08 21:34:37 by gbohm            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "libft.h"
#include "pipex.h"

int	main(int argc, const char **argv, char *const *envp)
{
	int	infile;

	if (argc != 5)
	{
		ft_putendl_fd("pipex: incorrect argument count", 2);
		return (1);
	}
	if (get_infile2(argv[1], &infile))
		return (2);
	if (envp == NULL)
		return (3);
	run(infile, 0, argv, envp);
	// system("leaks pipex");
	return (0);
}
