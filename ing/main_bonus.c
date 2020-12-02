#include "cub3d.h"

int	g_map_empty;

t_list	*ft_lstlast(t_list *lst)
{
	t_list	*temp;

	temp = lst;
	while (temp)
	{
		if (temp->next == 0)
			break ;
		temp = temp->next;
	}
	return (temp);
}

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*temp;

	temp = ft_lstlast(*lst);
	if (temp)
		temp->next = new;
	else
		*lst = new;
}

void	ft_lstdelone(t_list *lst)
{
	if (lst)
	{
		free(lst->content);
		free(lst);
	}
}

void	ft_lstclear(t_list **lst)
{
	t_list	*temp;

	while (*lst)
	{
		temp = (*lst)->next;
		ft_lstdelone(*lst);
		*lst = temp;
	}
}

t_list	*ft_lstnew(void *content)
{
	t_list	*temp;

	temp = (t_list *)malloc(sizeof(t_list));
	if (temp == 0)
		return (0);
	temp->content = content;
	temp->next = NULL;
	return (temp);
}

int	ft_lstsize(t_list *lst)
{
	t_list	*temp;
	int		count;

	temp = lst;
	count = 0;
	while (temp)
	{
		count++;
		temp = temp->next;
	}
	return (count);
}

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
		{
			j++;
		}
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

void	map_init(t_map *m)
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
	m->lst = 0;
}

int	info_null_check(t_map *m)
{
	if (m->north == 0 || m->south == 0 || m->west == 0 || m->east == 0)
		return (0);
	if (m->size == 0 || m->floor == 0 || m->ceil == 0 || m->item == 0)
		return (0);
	return (1);
}

int	info_empty_check(t_map *m)
{
	if (ft_strlen(m->north) == 0 || ft_strlen(m->south) == 0 ||
			ft_strlen(m->west) == 0 || ft_strlen(m->east) == 0)
		return (0);
	if (ft_strlen(m->size) == 0 || ft_strlen(m->floor) == 0 ||
			ft_strlen(m->ceil) == 0 || ft_strlen(m->item) == 0)
		return (0);
	return (1);
}

int	xpmfile_check(t_map *m)
{
	char	*c;

	c = ft_strnstr(m->north, ".xpm", ft_strlen(m->north));
	if (c == 0 || ft_strlen(c) != 4)
		return (0);
	c = ft_strnstr(m->south, ".xpm", ft_strlen(m->south));
	if (c == 0 || ft_strlen(c) != 4)
		return (0);
	c = ft_strnstr(m->west, ".xpm", ft_strlen(m->west));
	if (c == 0 || ft_strlen(c) != 4)
		return (0);
	c = ft_strnstr(m->east, ".xpm", ft_strlen(m->east));
	if (c == 0 || ft_strlen(c) != 4)
		return (0);
	c = ft_strnstr(m->item, ".xpm", ft_strlen(m->item));
	if (c == 0 || ft_strlen(c) != 4)
		return (0);
	c = ft_strnstr(m->floor, ".xpm", ft_strlen(m->floor));
	if (m->floor_color == -1 && (c == 0 || ft_strlen(c) != 4))
		return (0);
	c = ft_strnstr(m->ceil, ".xpm", ft_strlen(m->ceil));
	if (m->ceil_color == -1 && (c == 0 || ft_strlen(c) != 4))
		return (0);
	return (1);
}

int	map_line_check(t_map *m, char *line, int x)
{
	int	i;

	i = -1;
	while (line[++i] != '\0')
	{
		if (line[i] == 'N' || line[i] == 'S' ||
				line[i] == 'E' || line[i] == 'W')
		{
			if (m->start_dir == 0)
			{
				m->start_dir = line[i];
				m->start_x = x;
				m->start_y = i;
				line[i] = '0';
			}
			else
				return (0);
		}	// check plz
		else if (m->bonus_on == 0 && !((line[i] >= '0' && line[i] <= '2') || line[i] == 32))
			return (0);
		else if (m->bonus_on == 1 && !((line[i] >= '0' && line[i] <= '6') || line[i] == 32))
			return (0);
	}
	return (1);
}

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	get_color_n(char *line, int n, int i)
{
	while ((line[i] >= '0' && line[i] <= '9') || line[i] == 32)
		i++;
	if (line[i] != ',' || n > 255)
		return (-1);
	while (line[++i] == 32)
		;
	if (line[i] == ',' || line[i] == '\0')
		return (-1);
	return (i);
}

int	cal_color(t_map *m, char *line, int type)
{
	int	n;
	int	color;
	int	i;

	color = 0;
	n = ft_atoi(line);
	color += n * 256 * 256;
	if ((i = get_color_n(line, n, 0)) == -1)
		return (0);
	n = ft_atoi(line + i);
	color += n * 256;
	if ((i = get_color_n(line, n, i)) == -1)
		return (0);
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
	return (1);
}

int	floor_ceil_check(t_map *m, char *line, int type)
{
	int	i;

	if (line[0] == ',' || line[ft_strlen(line) - 1] == ',')
		return (0);
	i = 0;
	while ((line[i] >= '0' && line[i] <= '9') ||
			line[i] == ',' || line[i] == 32)
		i++;
	if (line[i] == '\0')
	{
		if (cal_color(m, line, type) == 0)
			return (0);
	}
	else if (m->bonus_on == 0)
		return (0);
	return (1);
}

int	space_check(char *line)
{
	int	i;

	i = 0;
	while (line[i] != '\0')
	{
		if (line[i] != 32)
			return (0);
		i++;
	}
	return (1);
}

void	eight_dir_check_init(int *i, int *k, int x, int y)
{
	*i = x - 1;
	*k = y - 1;
	if (*i < 0)
		*i = 0;
	if (*k < 0)
		*k = 0;
}

int	eight_dir_check(t_map *m, int x, int y)
{
	int i;
	int j;
	int k;
	int	p;
	int	q;

	eight_dir_check_init(&i, &k, x, y);
	p = x + 1;
	q = y + 1;
	if (p > m->map_x - 1)
		p = m->map_x - 1;
	if (q > m->map_y - 1)
		q = m->map_y - 1;
	while (i <= p)
	{
		j = k;
		while (j <= q)
		{
			if (!(m->map[i][j] == 32 || m->map[i][j] == 1))
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}

int	cubfile_open(t_map *m, int argc, char **argv)
{
	char	*c;

	c = ft_strnstr(argv[1], ".cub", ft_strlen(argv[1]));
	if (c == 0 || ft_strlen(c) != 4)
	{
		write(1, "first argument is no .cub file\n", 31);
		return (0);
	}
	m->fd = open(argv[1], O_RDONLY);
	if (m->fd < 0)
	{
		perror(argv[1]);
		return (0);
	}
	if (argc == 3)
	{
		c = ft_strnstr(argv[2], "--save", 6);
		if (c == 0 || ft_strlen(argv[2]) != 6)
		{
			write(1, "second argument is no \"--save\"\n", 31);
			return (0);
		}
		m->bitmap_check = 1;
	}
	return (1);
}

void	cubfile_info_free(t_map *m, int n);

int	info_check(t_map *m)
{
	if (floor_ceil_check(m, m->floor, 0) == 0)
	{
		write(1, "floor info error\n", 17);
		cubfile_info_free(m, 0);
		return (0);
	}
	if (floor_ceil_check(m, m->ceil, 1) == 0)
	{
		write(1, "ceil info error\n", 16);
		cubfile_info_free(m, 0);
		return (0);
	}
	if (info_empty_check(m) == 0 || xpmfile_check(m) == 0)
	{
		write(1, "one or more pieces of info are empty or not .xpm\n", 50);
		cubfile_info_free(m, 0);
		return (0);
	}
	return (1);
}

int	entering_info(char **info, char *data)
{
	*info = data;
	if (*info == 0)
	{
		write(1, "malloc allocation fail\n", 23);
		return (-1);
	}
	return (1);
}

int	gnl_info2(t_map *m, char *line)
{
	int	n;

	if (ft_strnstr(line, "NO ", 3) == line && m->north == 0)
		n = entering_info(&m->north, ft_strtrim(line + 2, " "));
	else if (ft_strnstr(line, "SO ", 3) == line && m->south == 0)
		n = entering_info(&m->south, ft_strtrim(line + 2, " "));
	else if (ft_strnstr(line, "WE ", 3) == line && m->west == 0)
		n = entering_info(&m->west, ft_strtrim(line + 2, " "));
	else if (ft_strnstr(line, "EA ", 3) == line && m->east == 0)
		n = entering_info(&m->east, ft_strtrim(line + 2, " "));
	else if (ft_strnstr(line, "S ", 2) == line && m->item == 0)
		n = entering_info(&m->item, ft_strtrim(line + 1, " "));
	else if (ft_strnstr(line, "F ", 2) == line && m->floor == 0)
		n = entering_info(&m->floor, ft_strtrim(line + 1, " "));
	else if (ft_strnstr(line, "C ", 2) == line && m->ceil == 0)
		n = entering_info(&m->ceil, ft_strtrim(line + 1, " "));
	else if (ft_strnstr(line, "R ", 2) == line && m->size == 0)
		n = entering_info(&m->size, ft_strtrim(line + 1, " "));
	else
		return (0);
	if (n == -1)
		cubfile_info_free(m, 0);
	free(line);
	return (n);
}

int	resolution_check(t_map *m)
{
	int	i;

	m->w = ft_atoi(m->size);
	i = 0;
	while (m->size[i] >= '0' && m->size[i] <= '9')
		i++;
	m->h = ft_atoi(m->size + i);
	while (m->size[i] == 32)
		i++;
	while (m->size[i] >= '0' && m->size[i] <= '9')
		i++;
	if (m->size[i] != '\0' || m->w == 0 || m->h == 0)
	{
		write(1, "resolution error\n", 17);
		cubfile_info_free(m, 0);
		return (0);
	}
	return (1);
}

void	cubfile_info_free(t_map *m, int n)
{
	if (m->north != 0)
		free(m->north);
	if (m->south != 0)
		free(m->south);
	if (m->west != 0)
		free(m->west);
	if (m->east != 0)
		free(m->east);
	if (m->item != 0)
		free(m->item);
	if (m->floor != 0)
		free(m->floor);
	if (m->ceil != 0)
		free(m->ceil);
	if (m->size != 0)
		free(m->size);
	if (n == -1)
		write(1, "error : get_next_line return value -1\n", 38);
}

int	info_full_check(t_map *m)
{
	if (info_null_check(m) == 0)
	{
		cubfile_info_free(m, 0);
		write(1, "cub file infomation error\n", 26);
		return (0);
	}
	return (1);
}

int	gnl_info(t_map *m, int n)
{
	char	*line;
	int		r;

	while (info_null_check(m) == 0 && (n = get_next_line(m->fd, &line)) > 0)
	{
		if ((r = gnl_info2(m, line)) == 0)
		{
			if (line[0] != '\0')
			{
				write(1, line, ft_strlen(line));
				write(1, " : this line error\n", 19);
				cubfile_info_free(m, 0);
				r = -1;
			}
			free(line);
		}
		if (r == -1)
			return (0);
	}
	if (n == -1)
		cubfile_info_free(m, -1);
	if (n == -1 || info_full_check(m) == 0 || info_check(m) == 0 || resolution_check(m) == 0)
		return (0);
	return (1);
}

void	cubfile_info_lst_free(t_map *m, int n)
{
	cubfile_info_free(m, n);
	ft_lstclear(&m->lst);
}

int	gnl_map_info_push_list(t_map *m, char *line)
{
	t_list *temp;

	if (g_map_empty == 1)
	{
		write(1, "map error : emptyline\n", 22);
		cubfile_info_lst_free(m, 0);
		free(line);
		return (0);
	}
	g_map_empty = 0;
	if ((temp = ft_lstnew(line)) == 0)
	{
		write(1, "malloc allocation fail\n", 23);
		cubfile_info_lst_free(m, 0);
		free(line);
		return (0);
	}
	ft_lstadd_back(&m->lst, temp);
	return (1);
}

int	gnl_map_error_check(t_map *m, char *line)
{
	if ((space_check(line) == 1 || line[0] == '\0') && g_map_empty == 0)
		g_map_empty = 1;
	else if ((space_check(line) == 1 || line[0] == '\0') && g_map_empty != 0)
		;
	else if (line[0] != '\0')
	{
		write(1, line, ft_strlen(line));
		write(1, " : this line map error\n", 23);
		cubfile_info_lst_free(m, 0);
		free(line);
		return (0);
	}
	free(line);
	return (1);
}

int	gnl_map_info(t_map *m, int n)
{
	char	*line;
	int		x;

	x = -1;
	while ((n = get_next_line(m->fd, &line)) > 0)
	{
		if (space_check(line) == 0 && map_line_check(m, line, ++x) == 1)
		{
			if (gnl_map_info_push_list(m, line) == 0)
				return (0);
		}
		else if (gnl_map_error_check(m, line) == 0)
			return (0);
	}
	if (n != -1 && m->start_dir == 0)
		write(1, "no starting direction error\n", 28);
	if (n == -1 || m->start_dir == 0)
	{
		cubfile_info_lst_free(m, 0);
		if (n == -1)
			write(1, "error : get_next_line return value -1\n", 38);
		return (0);
	}
	return (1);
}

void	get_map_size(t_map *m)
{
	t_list	*temp;

	temp = m->lst;
	m->map_x = ft_lstsize(m->lst);
	m->map_y = 0;
	while (temp)
	{
		if (ft_strlen(temp->content) > m->map_y)
			m->map_y = ft_strlen(temp->content);
		temp = temp->next;
	}
}

void	map_array_free(t_map *m, int n, int type)
{
	int	i;

	i = 0;
	while (i < n)
	{
		free(m->map[i]);
		i++;
	}
	free(m->map);
	cubfile_info_lst_free(m, 0);
	if (type == 0)
		write(1, "malloc allocation fail\n", 23);
}

int	fill_map_array2(t_map *m)
{
	t_list	*temp;
	char	*s;
	int		i;
	int		j;

	temp = m->lst;
	i = -1;
	while (++i < m->map_x)
	{
		if ((m->map[i] = (int *)malloc(sizeof(int) * m->map_y)) == 0)
			map_array_free(m, i, 0);
		if (m->map[i] == 0)
			return (0);
		j = -1;
		s = temp->content;
		while (++j < m->map_y)
		{
			if (j < ft_strlen(s) && ft_isdigit(s[j]) == 1)
				m->map[i][j] = s[j] - '0';
			else
				m->map[i][j] = 32;
		}
		temp = temp->next;
	}
	return (1);
}

int	fill_map_array(t_map *m)
{
	get_map_size(m);
	if ((m->map = (int **)malloc(sizeof(int *) * m->map_x)) == 0)
	{
		cubfile_info_lst_free(m, 0);
		write(1, "malloc allocation fail\n", 23);
		return (0);
	}
	if (fill_map_array2(m) == 0)
		return (0);
	return (1);
}

int	wall_check_up(t_map *m)
{
	int	i;
	int	j;

	i = 0;
	while (i < m->map_y)
	{
		j = 0;
		while (j < m->map_x)
		{
			if (m->map[j][i] == 1)
				break ;
			else if (m->map[j][i] == 0 || m->map[j][i] == 2)
			{
				write(1, "wall check up error\n", 20);
				return (0);
			}
			j++;
		}
		i++;
	}
	return (1);
}

int	wall_check_down(t_map *m)
{
	int	i;
	int	j;

	i = 0;
	while (i < m->map_y)
	{
		j = m->map_x - 1;
		while (j >= 0)
		{
			if (m->map[j][i] == 1)
				break ;
			else if (m->map[j][i] == 0 || m->map[j][i] == 2)
			{
				write(1, "wall check down error\n", 22);
				return (0);
			}
			j--;
		}
		i++;
	}
	return (1);
}

int	wall_check_left(t_map *m)
{
	int	i;
	int	j;

	i = 0;
	while (i < m->map_x)
	{
		j = 0;
		while (j < m->map_y)
		{
			if (m->map[i][j] == 1)
				break ;
			else if (m->map[i][j] == 0 || m->map[i][j] == 2)
			{
				write(1, "wall check left error\n", 22);
				return (0);
			}
			j++;
		}
		i++;
	}
	return (1);
}

int	wall_check_right(t_map *m)
{
	int	i;
	int	j;

	i = 0;
	while (i < m->map_x)
	{
		j = m->map_y - 1;
		while (j >= 0)
		{
			if (m->map[i][j] == 1)
				break ;
			else if (m->map[i][j] == 0 || m->map[i][j] == 2)
			{
				write(1, "wall check right error\n", 23);
				return (0);
			}
			j--;
		}
		i++;
	}
	return (1);
}

int	wall_eight_dir_check(t_map *m)
{
	int	i;
	int	j;

	i = 0;
	while (i < m->map_x)
	{
		j = 0;
		while (j < m->map_y)
		{
			if (m->map[i][j] == 32)
			{
				if (eight_dir_check(m, i, j) == 0)
				{
					write(1, "wall check eight dir check error\n", 33);
					return (0);
				}
			}
			j++;
		}
		i++;
	}
	return (1);
}

void	get_sprite_count(t_map *m)
{
	int	i;
	int	j;
	
	m->sprite_count = 0;
	i = 0;
	while (i < m->map_x)
	{
		j = 0;
		while (j < m->map_y)
		{
			if (m->map[i][j] == 2)
				m->sprite_count++;
			else if (m->bonus_on == 1 && (m->map[i][j] > 2 && m->map[i][j] <= 6))
				m->sprite_count++;
			j++;
		}
		i++;
	}
}

int	wall_check(t_map *m)
{
	int	n;

	n = 1;
	if (wall_check_up(m) == 0)
		n = 0;
	if (wall_check_down(m) == 0)
		n = 0;
	if (wall_check_left(m) == 0)
		n = 0;
	if (wall_check_right(m) == 0)
		n = 0;
	if (wall_eight_dir_check(m) == 0)
		n = 0;
	if (n == 0)
		map_array_free(m, m->map_x, 1);
	return (n);
}

int	main(int argc, char **argv)
{
	t_map	m;
	int		i;
	int		j;

	map_init(&m);
	m.bonus_on = 1;
	g_map_empty = -1;
	m.bitmap_check = 0;
	if (argc == 2 || argc == 3)
	{
		if (cubfile_open(&m, argc, argv) == 0)
			return (0);
		if (gnl_info(&m, 0) == 0)
		{
			close(m.fd);
			return (0);
		}
		if (gnl_map_info(&m, 0) == 0)
		{
			close(m.fd);
			return (0);
		}
		if (close(m.fd) < 0)
		{
			write(1, "close error\n", 12);
			return (0);
		}
		if (fill_map_array(&m) == 0)
			return (0);
		if (wall_check(&m) == 0)
			return (0);
		get_sprite_count(&m);
		cub_play(&m);

		printf(" ---------------\n");
		printf("dir %c x = %d y = %d\n", m.start_dir, m.start_x, m.start_y);
		for (i = 0; i < m.map_x; i++)
		{
			for (j = 0; j < m.map_y; j++)
			{
				if (m.map[i][j] == 32)
					printf("@");
				else
					printf("%d", m.map[i][j]);
			}
			printf("\n");
		}
		printf("map x = %d map y = %d\n", m.map_x, m.map_y);
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
		printf("w = %d h = %d\n", m.w, m.h);
	}
	else
		write(1, "argument error : program [.cub] ([--save]) is not\n", 50);
	return (0);
}
