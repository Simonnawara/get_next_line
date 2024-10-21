/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 15:42:37 by sinawara          #+#    #+#             */
/*   Updated: 2024/10/21 14:43:10 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

// Extract a line from the stash up to the first newline or end
char	*extract_line(char *stash)
{
	size_t	i;
	char	*line;

	i = 0;
	while (stash[i] && stash[i] != '\n')
		i++;
	if (stash[i] == '\n')
		line = malloc(sizeof(char) * (i + 2)); //adds place for '\n' && '\0'
	else
		line = malloc(sizeof(char) * (i + 1));//adds place for '\0'
	if (!line)
		return (NULL);
	i = 0;
	while (stash[i] && stash[i] != '\n')
	{
		line[i] = stash[i];
		i++;
	}
	if (stash[i] == '\n')
	{
		line[i] = stash[i];
		i++;
	}
	line[i] = '\0';
	return (line);
}

// Save the leftover content after the first newline
char	*save_leftover(char *stash)
{
	char	*leftover;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (stash[i] && stash[i] != '\n')
		i++;
	if (!stash[i])
	{
		free(stash);
		return (NULL);
	}
	leftover = malloc(ft_strlen(stash) - i + 1);
	if (!leftover)
		return (NULL);
	i++;
	while (stash[i])
		leftover[j++] = stash[i++];
	leftover[j] = '\0';
	free(stash);
	return (leftover);
}

char	*get_next_line(int fd)
{
	static char	*stash;
	char		buffer[BUFFER_SIZE + 1];
	char		*temp;
	char		*line;
	ssize_t		bytes_read;

	if (fd < 0 || BUFFER_SIZE <= 0 || BUFFER_SIZE > 2147483647)
		return (NULL);
	bytes_read = read(fd, buffer, BUFFER_SIZE);
	while (bytes_read >= 0)
	{
		buffer[bytes_read] = '\0';
		temp = stash;
		stash = ft_strjoin(stash, buffer);
		free(temp);
		if (ft_strchr(stash, '\n'))
		{
			line = extract_line(stash);
			stash = save_leftover(stash);
			return (line);
		}
		bytes_read--;
	}
	if (!stash || !*stash)
    {
        free(stash);
        stash = NULL;
    }
	if (stash && *stash)
	{
		line = extract_line(stash);
		stash = NULL;
		return (line);
	}
	return (NULL);
}
int	main(void)
{
	int	fd = open("t.txt", O_RDONLY);
	char	*line;

	if (fd == -1)
	{
		perror("Error opening file");
		return (1);
	}

	while ((line = get_next_line(fd)) != NULL)
	{
		printf("%s", line);
		free(line);
	}

	close(fd);
	system("leaks a.out");
	return (0);
}
