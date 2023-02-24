/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pl_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wricky-t <wricky-t@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 11:10:28 by wricky-t          #+#    #+#             */
/*   Updated: 2023/02/24 11:12:01 by wricky-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

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
