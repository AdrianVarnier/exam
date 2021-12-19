#include "get_next_line.h"

#include <stdio.h>

static int	is_newline(char *s)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

static int	ft_strlen(char *s, char c)
{
	int	i;

	i = 0;
	while (s[i] != c)
		i++;
	if (c == '\n')
		i++;
	return (i);
}

static char *ft_strdup(char *s, char c)
{
	int		i;
	char	*cpy;

	i = 0;
	cpy = malloc(sizeof(char) * (ft_strlen(s, c) + 1));
	while (s[i] != c)
	{
		cpy[i] = s[i];
		i++;
	}
	if (c == '\n')
	{
		cpy[i] = '\n';
		i++;
	}
	cpy[i] = '\0';
	return (cpy);
}

static char	*ft_strjoin(char *s1, char *s2, char c)
{
	int		i;
	int		j;
	char	*cpy;

	i = 0;
	j = 0;
	if (s1 == NULL)
		return (ft_strdup(s2, c));
	cpy = malloc(sizeof(char) * (ft_strlen(s1, '\0') + ft_strlen(s2, c) + 1));
	while (s1[i] != '\0')
	{
		cpy[i] = s1[i];
		i++;
	}
	while (s2[j] != c)
	{
		cpy[i] = s2[j];
		i++;
		j++;
	}
	if (c == '\n')
	{
		cpy[i] = '\n';
		i++;
	}
	cpy[i] = '\0';
	return (cpy);
}

static void	ft_memmove(char *dst, char *src, int len)
{
	int	i;

	i = 0;
	if (src == dst)
		return ;
	while (i < len)
	{
		dst[i] = src[i];
		i++;
	}
}

char	*get_next_line(int fd)
{
	int			ret;
	char		*line;
	char		*tmp;
	static char	buffer[BUFFER_SIZE + 1];

	ret = 0;
	line = NULL;
	if (fd < 0 || BUFFER_SIZE < 0)
		return (NULL);
	if (buffer[0] != '\0')
	{
		if (is_newline(buffer) == 1)
		{
			line = ft_strdup(buffer, '\n');
			ft_memmove(buffer, buffer + ft_strlen(buffer, '\n'), ft_strlen(buffer, '\0') - ft_strlen(buffer, '\n') + 1);
			return (line);
		}
		else
		{
			line = ft_strdup(buffer, '\0');
//			ft_bzero(buffer, BUFFER_SIZE + 1);
			buffer[0] = '\0';
		}
	}
	while ((ret = read(fd, buffer, BUFFER_SIZE)) > 0)
	{
		buffer[ret] = '\0';
		if (is_newline(buffer) == 1)
		{
			tmp = line;
			line = ft_strjoin(line, buffer, '\n');
			ft_memmove(buffer, buffer + ft_strlen(buffer, '\n'), ft_strlen(buffer, '\0') - ft_strlen(buffer, '\n') + 1);
			if (tmp != NULL)
				free(tmp);
			return (line);
		}
		else
		{
			tmp = line;
			line = ft_strjoin(line, buffer, '\0');
			if (tmp != NULL)
				free(tmp);
//			ft_bzero(buffer, BUFFER_SIZE + 1);
			buffer[0] = '\0';
		}
	}
	if (ret == 0 && buffer[0] == '\0' && line == NULL)
		return (NULL);
	if (ret == -1)
		return (NULL);
	return (line);
}
