/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pl_parse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wricky-t <wricky-t@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 11:08:38 by wricky-t          #+#    #+#             */
/*   Updated: 2023/02/25 16:38:58 by wricky-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/**
 * @brief Convert string to integer
 * @param str The str that needs to be converted
 *
 * @return The num in int data type
 */
static int	ft_atoi(const char *str)
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
static int	ft_isdigit_str(char *str)
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
 * @brief Check the arguments
 * @param ac argument count (not include program name)
 * @param av program arguments (not include program name)
 *
 * @details
 * Iterate through the args and check if the arg only contain digits.
 * If not, show error and return 0.
 * After confirm the arg contains only digits characters, convert it
 * to numeric value. Check if it's a negative value or not.
 * If true, show error and return 0.
 *
 * @return
 * If each of the args is ok, return 1; Return 0 if there's an error.
 */
static int	pl_check_args(int ac, char **av)
{
	int	i;
	int	arg;

	i = -1;
	while (++i < ac)
	{
		if (ft_isdigit_str(av[i]) == 0)
			return (pl_show_error(NON_NUMERIC_ARGS, 0));
		arg = ft_atoi(av[i]);
		if (arg < 0)
			return (pl_show_error(NEGATIVE_ARGS, 0));
		if (arg == 0 && i == 4)
			return (pl_show_error(INVALID_OPTION, 0));
	}
	return (1);
}

/**
 * @brief Set up simulation rules
 * @param ac Argument count (not including program name)
 * @param av Arguments (not including program name)
 * @param rules The rules struct
 */
static void	pl_setup_rules(int ac, char **av, t_rules *rules)
{
	rules->philo_total = ft_atoi(*av++);
	rules->time_to_die = ft_atoi(*av++);
	rules->time_to_eat = ft_atoi(*av++);
	rules->time_to_sleep = ft_atoi(*av++);
	if (ac == 5)
		rules->iteration = ft_atoi(*av);
	else
		rules->iteration = 0;
}

/**
 * @brief Parse the arguments and store it into rules struct
 * @param ac Argument count (not including program name)
 * @param av Arguments (not including program name)
 * @param rules The rules struct
 *
 * @details
 * 1. Initialize the rules struct
 * 2. If the ac is not 4 nor 5, show error and return
 * 3. Check args
 * 4. Set simulation info
 *
 * @return Return 1 if the parse process is successful, 0 otherwise.
 */
int	pl_parse(int ac, char **av, t_rules *rules)
{
	if (ac != 4 && ac != 5)
		return (pl_show_error(INVALID_ARGS_TOTAL, 0));
	if (pl_check_args(ac, av) == 0)
		return (0);
	pl_setup_rules(ac, av, rules);
	return (1);
}
