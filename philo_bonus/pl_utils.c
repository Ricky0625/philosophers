/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pl_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wricky-t <wricky-t@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 11:10:28 by wricky-t          #+#    #+#             */
/*   Updated: 2023/02/25 16:34:55 by wricky-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/**
 * @brief Get current time in seconds
 *
 * @return Time in time_t format
 */
time_t	pl_get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

/**
 * @brief A better usleep
 *
 * @details
 * Through some experiments as well as peer's observation, it seems like
 * usleep is not that accurate in some case. Usleep might "overslept", which
 * makes the timestamp of the program less accurate. Hence, to prevent this
 * situation, the idea is to let usleep to sleep a constant amount of time
 * and check if the program has slept for what's required.
 */
void	pl_usleep(time_t sec)
{
	time_t	cur_time;

	cur_time = pl_get_time();
	while (1)
	{
		usleep(500);
		if ((pl_get_time() - cur_time) >= sec)
			break ;
	}
}

/**
 * @brief Convert numbers into string
*/
char	*ft_itoa(int n)
{
	size_t		i;
	long int	an;
	char		*output;

	an = n;
	i = (n <= 0);
	while (n != 0 && ++i > 0)
		n = n / 10;
	output = malloc(sizeof(char) * (i + 1));
	if (output == NULL)
		return (NULL);
	if (an < 0)
		output[0] = '-';
	if (an < 0)
		an *= -1;
	output[i--] = '\0';
	if (an == 0)
		output[0] = 48;
	while (an)
	{
		output[i--] = (an % 10) + '0';
		an /= 10;
	}
	return (output);
}

/**
 * @brief Get the length of a string
*/
size_t	ft_strlen(const char *str)
{
	size_t	len;

	len = 0;
	while (*str)
	{
		len++;
		str++;
	}
	return (len);
}

/**
 * @brief Join two string together
*/
char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	size_t	size;
	size_t	index;

	if (!s1 || !s2)
		return (NULL);
	size = ft_strlen(s1) + ft_strlen(s2);
	str = malloc(sizeof(char) * (size + 1));
	if (str == NULL)
		return (NULL);
	index = 0;
	while (*s1)
		*(str + index++) = *s1++;
	while (*s2)
		*(str + index++) = *s2++;
	*(str + index) = '\0';
	return (str);
}
