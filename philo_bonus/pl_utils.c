/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pl_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wricky-t <wricky-t@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 10:38:17 by wricky-t          #+#    #+#             */
/*   Updated: 2023/02/23 12:05:25 by wricky-t         ###   ########.fr       */
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
