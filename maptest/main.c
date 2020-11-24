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
	int		floor_color;
	char	*ceil;
	int		ceil_color;
	char	*one;
	char	start_dir;
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
	m->floor_color = -1;
	m->ceil = 0;
	m->ceil_color = -1;
	m->w = 0;
	m->h = 0;
	m->start_dir = 0;
}

int	element_check(t_map *m)
{
	if (m->north == 0 || m->south == 0 || m->west == 0 || m->east == 0)
	   return (0);
	if (m->size == 0 || m->floor == 0 || m->ceil == 0 || m->item == 0)
		return (0);
	return (1);	
}

int	element_size_check(t_map *m)
{
	if (ft_strlen(m->north) == 0 || ft_strlen(m->south) == 0 || ft_strlen(m->west) == 0 || ft_strlen(m->east) == 0)
		return (0);
	if (ft_strlen(m->size) == 0 || ft_strlen(m->floor)== 0 || ft_strlen(m->ceil) == 0 || ft_strlen(m->item) == 0)
		return (0);
	return (1);
}

int	map_line_check(t_map *m, char *line)
{
	int	i;

	if (line[0] == '\0')
		return (0);
	i = 0;
	while (line[i] != '\0')
	{
		if (line[i] == 'N' || line[i] == 'S' || line[i] == 'E' || line[i] == 'W')
		{
			if (m->start_dir == 0)
			{
				m->start_dir = line[i];
				// start x , y setting 
				// line[i] = 0;
			}
			else
				return (0);
		}
		else if (!((line[i] >= '0' && line[i] <= '9') || line[i] == 32))
			return (0);
		i++;
	}
	return (1);
}

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	floor_ceil_check(t_map *m, char *line, int type)
{
	int	i;
	int	color;
	int	n;

	i = 0;
	color = 0;
	while ((line[i] >= '0' && line[i] <= '9') || line[i] == ',')
	{
		if (line[i] == ',' && (i == 0 || i == (ft_strlen(line) - 1) || ft_isdigit(line[i - 1]) == 0 || ft_isdigit(line[i + 1]) == 0))
			return (0);
		i++;
	}
	if (line[i] == '\0')
	{
		n = ft_atoi(line);
		color = n * 256 * 256;
		i = 0;
		while (line[i] >= '0' && line[i] <= '9')
			i++;
		if (line[i] == '\0' || n > 255)
			return (0);
		i++;
		n = ft_atoi(line + i);
		color += n * 256;
		while (line[i] >= '0' && line[i] <= '9')
			i++;
		if (line[i] == '\0' || n > 255)
			return (0);
		i++;
		n = ft_atoi(line + i);
		color += n;
		while (line[i] >= '0' && line[i] <= '9')
			i++;
		if (line[i] != '\0' || n > 255)
			return (0);
		if (type == 0)
			m->floor_color = color;
		else if (type == 1)
			m->ceil_color = color;
	}
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

	map_init(&m);
	if (argc == 1 || argc == 2)
	{
		fd = open(argv[1], O_RDONLY);
		if (fd < 0)
		{
			printf("file name error\n");
			return (0);
		}
		while (get_next_line(fd, &line) > 0)
		{
			if (ft_strnstr(line, "NO", 2) == line && m.north == 0)	
				m.north = ft_strtrim(line + 2, " ");
			else if (ft_strnstr(line, "SO ", 3) == line && m.south == 0)
				m.south = ft_strtrim(line, "SO ");
			else if (ft_strnstr(line, "WE ", 3) == line && m.west == 0)
				m.west = ft_strtrim(line, "WE ");
			else if (ft_strnstr(line, "EA ", 3) == line && m.east == 0)
				m.east = ft_strtrim(line, "EA ");
			else if (ft_strnstr(line, "S ", 2) == line && m.item == 0)
				m.item = ft_strtrim(line, "S ");
			else if(ft_strnstr(line, "F ", 2) == line && m.floor == 0)
				m.floor = ft_strtrim(line, "F ");
			else if (ft_strnstr(line, "C ", 2) == line && m.ceil == 0)
				m.ceil = ft_strtrim(line, "C ");
			else if (ft_strnstr(line, "R", 2) == line && m.size == 0)
				m.size = ft_strtrim(line, "R ");
			else
			{
				if (line[0] != '\0')
				{
					write(1, "error line : ", 13);
					write(1, line, ft_strlen(line));
					write(1, "\n", 1);
					return (0);
				}
			}
			if (element_check(&m) == 1)
				break ;
		}
		if (element_size_check(&m) == 0)
		{
			printf("error\n");
			return (0);
		}
		if (floor_ceil_check(&m, m.floor, 0) == 0)
		{
			printf("error\n");
			return (0);
		}
		if (floor_ceil_check(&m, m.ceil, 1) == 0)
		{
			printf("error\n");
			return (0);
		}
		m.one = ft_strdup("");
		while (get_next_line(fd, &line) > 0)
		{
			if (map_line_check(&m, line) == 1)
			{
				y = ft_strlen(line);
				temp = ft_strjoin(m.one, line);
				free(m.one);
				m.one = temp;
			}
			else
			{
				if (line[0] != '\0')
				{
					write(1, "error line : ", 13);
					write(1, line, ft_strlen(line));
					write(1, "\n", 1);
					free(m.one);
					return (0);
				}
			}
		}
		if (m.start_dir == 0)
		{
			printf("start_dir error\n");
			return (0);
		}
		x = ft_strlen(m.one) / y;
		m.map = (int **)malloc(sizeof(int *) * x);
		for (int i = 0;	i < y; i++)
			m.map[i] = (int *)malloc(sizeof(int) * y);
		for (int i = 0; i < x; i++)
		{
			for (int j = 0; j < y; j++)
			{
				if (ft_isdigit(m.one[i * y + j]) == 1)
					m.map[i][j] = m.one[i * y + j] - '0';
				else
					m.map[i][j] = m.one[i * y + j];
			}	
		}

		for (int i = 0; i < x; i++)
		{
			for (int j = 0; j < y; j++)
				printf("%d", m.map[i][j]);
			printf("\n");
		}
	}
	
	printf("%d %x\n", m.floor_color, m.floor_color);
	printf("%d %x\n", m.ceil_color, m.ceil_color);
	
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
	i++;
	while (m.size[i] >= '0' && m.size[i] <= '9')
		i++;
	if (m.size[i] != '\0' || m.w == 0 || m.h == 0)
		printf("m.w or m.h is zero\n");
	printf("w = %d h = %d\n", m.w, m.h);
	return (0);
}
