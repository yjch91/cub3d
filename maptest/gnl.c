#include "gnl.h"

static	int	find_save(char **save, char **line)
{
	int		i;
	char	*temp;

	i = 0;
	if ((*save) != 0)
	{
		while ((*save)[i] != '\0' && (*save)[i] != '\n')
			i++;
		if ((*save)[i] != '\n')
			return (0);
		if ((*line = ft_strdup_size(*save, 0, i)) == 0)
		{
			free(*save);
			return (-1);
		}
		if ((temp = ft_strdup_size(*save, i + 1, ft_strlen(*save))) == 0)
		{
			free(*save);
			return (-1);
		}
		free(*save);
		*save = temp;
		return (1);
	}
	return (0);
}

static int	save_plus_buf(char **save, char *buf, int n)
{
	char	*temp;

	if (*save != 0)
	{
		temp = ft_strjoin(*save, buf);
		free(*save);
		if (temp == 0)
			return (-1);
		*save = temp;
	}
	else
	{
		if ((*save = ft_strdup_size(buf, 0, n)) == 0)
			return (-1);
	}
	return (0);
}

static int	read_line(int fd, char **line, char **save)
{
	int		n;
	int		m;
	char	*buf;

	if ((buf = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1))) == 0)
		return (-1);
	while ((n = read(fd, buf, BUFFER_SIZE)) > 0)
	{
		buf[n] = '\0';
		if ((m = save_plus_buf(&(*save), buf, n)) == -1)
		{
			free(buf);
			return (-1);
		}
		if ((m = find_save(&(*save), &(*line))) != 0)
		{
			free(buf);
			return (m);
		}
	}
	free(buf);
	return (n);
}

static int	rest_clear(char **save, char **line)
{
	if (*save != 0)
	{
		if ((*line = ft_strjoin("", *save)) == 0)
		{
			free(*save);
			*save = NULL;
			return (-1);
		}
	}
	else if (*save == 0)
	{
		if ((*line = (char *)malloc(sizeof(char))) == 0)
			return (-1);
		(*line)[0] = '\0';
	}
	return (0);
}

int			get_next_line(int fd, char **line)
{
	int			n;
	static char	*save[10240];

	if (fd < 0 || BUFFER_SIZE <= 0 || line == 0)
		return (-1);
	if (save[fd] != 0)
	{
		if ((n = find_save(&save[fd], &(*line))) != 0)
			return (n);
	}
	n = read_line(fd, &(*line), &save[fd]);
	if (n == 1)
		return (1);
	if (n == 0)
		n = rest_clear(&save[fd], &(*line));
	if (save[fd] != 0)
		free(save[fd]);
	save[fd] = NULL;
	return (n);
}

