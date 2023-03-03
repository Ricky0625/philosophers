/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pl_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wricky-t <wricky-t@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 11:10:28 by wricky-t          #+#    #+#             */
/*   Updated: 2023/03/03 18:21:38 by wricky-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/**
 * @brief Get timestamp
 * 
 * @details
 * Get the current time and minus the target time. Return
 * the value difference.
 * 
 * @attention
 * In my mandatory, I didn't use the struct timeval directly. I convert
 * the time format into time_t because I think that's easier to read.
 * However, this method will cause data race in my bonus part even though
 * I protected all the critical part. I don't understand why and how.
 * Thankfully, Sean the KHONVOUM, went through my code and found this
 * issue. The reason behind this is unknown. It's so bizzare.
*/
int	pl_get_timestamp(struct timeval before)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	return (((now.tv_sec - before.tv_sec) * 1000)
		+ ((now.tv_usec - before.tv_usec) / 1000));
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
void	pl_usleep(int sec)
{
	struct timeval	start;

	gettimeofday(&start, NULL);
	while (1)
	{
		usleep(500);
		if (pl_get_timestamp(start) >= sec)
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
