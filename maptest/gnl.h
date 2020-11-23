#ifndef GNL_H

# define GNL_H

# include <stdlib.h>
# include <unistd.h>

# define	BUFFER_SIZE 100

int		get_next_line(int fd, char **line);
char	*ft_strdup_size(const char *s1, int left, int right);
int		ft_strlen(const char *s);
char	*ft_strjoin(char const *s1, char const *s2);

#endif
