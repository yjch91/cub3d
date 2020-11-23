#include <stdio.h>
#include <fcntl.h>
#include "gnl.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	int	i;
	int	j;
	int	n_len;

	i = 0;
	j = 0;
	n_len = ft_strlen(needle);
	if (n_len == 0)
		return ((char *)(haystack));
	while (haystack[i] && (unsigned long)i < len)
	{
		if (haystack[i] == needle[j])
			j++;
		else
		{
			i -= j;
			j = 0;
		}
		if (j == n_len)
			return ((char *)(haystack + i - j + 1));
		i++;
	}
	return (0);
}

char	*ft_strdup(const char *s1)
{
	int		len;
	int		i;
	char	*temp;

	len = 0;
	while (s1[len] != '\0')
		len++;
	temp = (char *)malloc(sizeof(char) * (len + 1));
	if (temp == 0)
		return (0);
	temp[len] = '\0';
	i = 0;
	while (i < len)
	{
		temp[i] = s1[i];
		i++;
	}
	return (temp);
}

static	int	set_check(char const *set, char c)
{
	int	i;

	i = 0;
	while (set[i] != '\0')
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char		*ft_strtrim(char const *s1, char const *set)
{
	char	*temp;
	int		i;
	int		j;
	int		k;

	if (s1 != 0 && set != 0)
	{
		i = 0;
		while (s1[i] != '\0' && (set_check(set, s1[i]) == 1))
			i++;
		j = ft_strlen(s1);
		while (j > i && (set_check(set, s1[j - 1]) == 1))
			j--;
		temp = (char *)malloc(sizeof(char) * (j - i + 1));
		if (temp == 0)
			return (0);
		k = 0;
		while (i < j)
			temp[k++] = s1[i++];
		temp[k] = '\0';
		return (temp);
	}
	return ((char *)s1);
}

int	ft_atoi(const char *str)
{
	int			i;
	int			minus;
	long long	result;

	i = 0;
	minus = 1;
	result = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-')
		minus = -1;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (result >= 922337203685477580 && str[i] > '7' && minus == 1)
			return (-1);
		else if (result >= 922337203685477580 && str[i] > '8' && minus == -1)
			return (0);
		result = result * 10 + str[i++] - '0';
	}
	return ((int)result * minus);
}

typedef struct	s_map
{
	char	*north;
	char	*south;
	char	*west;
	char	*east;
	char	*size;
	int		w;
	int		h;
	char	*item;
	char	*floor;
	char	*ceil;
	char	*one;
	int		**map;
}				t_map;

void map_init(t_map *m)
{
	m->north = 0;
	m->south = 0;
	m->west = 0;
	m->east = 0;
	m->size = 0;
	m->item = 0;
	m->floor = 0;
	m->ceil = 0;
	m->w = 0;
	m->h = 0;
	//m->one = ft_strdup("");
}

int	element_check(t_map *m)
{
	if (m->north == 0 || m->south == 0 || m->west == 0 || m->east == 0)
	   return (0);
	if (m->size == 0 || m->floor == 0 || m->ceil == 0)
		return (0);
	return (1);	
}

int	main(int argc, char **argv)
{
	int		fd;
	char	*line;
	t_map	m;
	char	*temp;
	int		x;
	int		y;

	if (argc == 1 || argc == 2)
	{
		fd = open(argv[1], O_RDONLY);
		while (get_next_line(fd, &line) > 0)
		{
			if (ft_strnstr(line, "NO", 2) != 0)	
				m.north = ft_strtrim(line, "NO ");
			if (ft_strnstr(line, "SO", 2) != 0)
				m.south = ft_strtrim(line, "SO ");
			if (ft_strnstr(line, "WE", 2) != 0)
				m.west = ft_strtrim(line, "WE ");
			if (ft_strnstr(line, "EA", 2) != 0)
				m.east = ft_strtrim(line, "EA ");
			if (ft_strnstr(line, "S", 1) != 0)
				m.item = ft_strtrim(line, "S ");
			if (ft_strnstr(line, "F", 1) != 0)
				m.floor = ft_strtrim(line, "F ");
			if (ft_strnstr(line, "C", 1) != 0)
				m.ceil = ft_strtrim(line, "C ");
			if (ft_strnstr(line, "R", 1) != 0)
				m.size = ft_strtrim(line, "R ");
			if (element_check(&m) == 1)
				break ;
		}
		m.one = ft_strdup("");
		while (get_next_line(fd, &line) > 0)
		{
			if (ft_strnstr(line, "1", 1)  != 0 || ft_strnstr(line, "0", 1) != 0)
			{
				y = ft_strlen(line);
				temp = ft_strjoin(m.one, line);
				free(m.one);
				m.one = temp;
			}	
		}
		x = ft_strlen(m.one) / y;
		m.map = (int **)malloc(sizeof(int *) * x);
		for (int i = 0;	i < y; i++)
			m.map[i] = (int *)malloc(sizeof(int) * y);
		for (int i = 0; i < x; i++)
		{
			for (int j = 0; j < y; j++)
			{
				m.map[i][j] = *m.one++ - '0';
			}	
		}

		for (int i = 0; i < x; i++)
		{
			for (int j = 0; j < y; j++)
				printf("%d", m.map[i][j]);
			printf("\n");
		}
	}
	printf("%s\n", m.north);
	printf("%s\n", m.south);
	printf("%s\n", m.west);
	printf("%s\n", m.east);
	printf("%s\n", m.item);
	printf("%s\n", m.floor);
	printf("%s\n", m.ceil);
	printf("%s\n", m.size);
	m.w = ft_atoi(m.size);
	int i = 0;
	while (m.size[i] >= '0' && m.size[i] <= '9')
		i++;
	m.h = ft_atoi(m.size + i);
	printf("w = %d h = %d\n", m.w, m.h);
	return (0);
}
