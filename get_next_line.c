/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnakai <hnakai@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 00:59:19 by hnakai            #+#    #+#             */
/*   Updated: 2023/06/10 11:01:54 by hnakai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_readbuff(char *buff, int fd)
{
	ssize_t	read_size;

	read_size = read(fd, buff, BUFFER_SIZE);
	if (read_size <= 0)
	{
		free(buff);
		return (NULL);
	}
	buff[read_size] = '\0';
	return (buff);
}

size_t	ft_count(char *buff)
{
	size_t	i;

	i = 0;
	if (!buff)
		return (0);
	while (buff[i] != '\0')
	{
		if (buff[i] == '\n')
			return (i);
		i++;
	}
	return (i);
}

char	*ft_make_buff(char *save, int fd)
{
	char	*buff;
	size_t	i;
	size_t	n;

	i = 0;
	n = BUFFER_SIZE;
	buff = (char *)malloc(sizeof(char) * (n + 1));
	if (!buff)
		return (NULL);
	if (!save)
	{
		buff = ft_readbuff(buff, fd);
		// printf("%s", buff);
		if (!buff)
			return (NULL);
	}
	else
	{
		i = -1;
		while (save[++i] != '\0')
			buff[i] = save[i];
		buff[i] = '\0';
		free(save);
	}
	return (buff);
}

char	*ft_make_save(char *buff)
{
	char	*save;

	save = ft_substr(buff, ft_count(buff) + 1, ft_strlen(buff) - ft_count(buff)
			+ 1);
	free(buff);
	if (!save && !save[0])
	{
		free(save);
		save = NULL;
	}
	return (save);
}

char	*get_next_line(int fd)
{
	char		*buff;
	static char	*save;
	char		*line;
	char		*arr;

	line = NULL;
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buff = ft_make_buff(save, fd);
	if (!buff)
		return (NULL);
	save = NULL;
	while (ft_count(buff) == ft_strlen(buff))
	{
		// printf("%s", buff);
		line = ft_strjoin(line, buff);
		buff = ft_readbuff(buff, fd);
		if (!buff)
			return (line);
	}
	arr = ft_substr(buff, 0, ft_count(buff) + 1);
	line = ft_strjoin(line, arr);
	free(arr);
	save = ft_make_save(buff);
	return (line);
}

#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int	main(void)
{
	int		fd;
	char	*line;

	// "text.txt" ファイルを開く
	fd = open("test.txt", O_RDONLY);
	if (fd == -1)
	{
		perror("Error opening file");
		return (1);
	}
	// get_next_lineを使ってファイルから一行ずつ読み込み、それを表示する
	while ((line = get_next_line(fd)) != NULL)
	{
		// printf("%s", line);
		free(line);
	}
	// ファイルディスクリプタを閉じる
	if (close(fd) == -1)
	{
		perror("Error closing file");
		return (1);
	}
	return (0);
}
