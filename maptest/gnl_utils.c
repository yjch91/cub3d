#include "gnl.h"

char	*ft_strdup_size(const char *s1, int left, int right)
{
	int		i;
	char	*temp;

	temp = (char *)malloc(sizeof(char) * (right - left + 1));
	if (temp == 0)
		return (0);
	temp[right - left] = '\0';
	i = 0;
	while (i < (right - left))
	{
		temp[i] = s1[left + i];
		i++;
	}
	return (temp);
}

int		ft_strlen(const char *s)
{
	int	len;

	len = 0;
	while (s[len] != '\0')
		len++;
	return (len);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*temp;
	int		len;
	int		len2;
	int		i;
	int		j;

	temp = 0;
	if (s1 != 0 && s2 != 0)
	{
		len = ft_strlen(s1);
		len2 = ft_strlen(s2);
		temp = (char *)malloc(sizeof(char) * (len + len2 + 1));
		if (temp == 0)
			return (0);
		temp[len + len2] = '\0';
		i = -1;
		while (++i < len)
			temp[i] = s1[i];
		j = -1;
		while ((++j + i) < (len + len2))
			temp[i + j] = s2[j];
	}
	return (temp);
}
