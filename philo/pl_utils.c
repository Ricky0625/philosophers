/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pl_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wricky-t <wricky-t@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 10:38:17 by wricky-t          #+#    #+#             */
/*   Updated: 2023/02/20 18:07:42 by wricky-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Convert string to integer
 * @param str The str that needs to be converted
 * 
 * @return The num in int data type
*/
int	ft_atoi(const char *str)
{
	int	sign;
	int	num;

	sign = 1;
	num = 0;
	while (*str == 32 || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -sign;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		num = (num * 10) + (*str - '0');
		str++;
	}
	return (sign * num);
}

/**
 * @brief Check if a string only consists of numeric number,
 * 	      can start with '-' or '+'.
 * 
 * @return 0 if the string consists character that are not digit.
 *         1 if the string only consists of digit character.
*/
int	ft_isdigit_str(char *str)
{
	int	sign;

	if (*str == '+' || *str == '-')
	{
		sign = 1;
		str++;
	}
	while (*str != '\0')
	{
		sign = 0;
		if ((*str >= '0' && *str <= '9') == 0)
			return (0);
		str++;
	}
	if (sign == 1)
		return (0);
	return (1);
}

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
