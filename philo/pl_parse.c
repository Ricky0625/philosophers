/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pl_parse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wricky-t <wricky-t@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 10:45:21 by wricky-t          #+#    #+#             */
/*   Updated: 2023/02/19 16:14:57 by wricky-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Setup additional locks that prevent data race
 * @param locks The locks struct
 * @param id the philo id
 * 
 * @details
 * 1. Last ate lock - Philo's last ate time.
 * 2. Meal count lock - Philo's meal count.
 * 3. Declare lock - Philo's state declaration.
 * 4. Sim state lock - Simulation's state.
 * 
 * @return
 * 1, If all additional locks can be initialized.
 * 0, If there's an error while creating a mutex.
*/
int	pl_lock_setup(t_locks *locks, t_lock_type type)
{
	if (type != SHARED && type != PHILO)
		return (0);
	if (type == SHARED)
	{
		if (pthread_mutex_init(&locks->declare_lock, NULL) != 0)
			return (pl_show_error(CREATE_MUT_FAILED, -1));
		if (pthread_mutex_init(&locks->sim_state_lock, NULL) != 0)
			return (pl_show_error(CREATE_MUT_FAILED, -1));
		return (1);
	}
	if (pthread_mutex_init(&locks->last_ate_lock, NULL) != 0)
		return (pl_show_error(CREATE_MUT_FAILED, -1));
	if (pthread_mutex_init(&locks->meal_count_lock, NULL) != 0)
		return (pl_show_error(CREATE_MUT_FAILED, -1));
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
	}
	return (1);
}

/**
 * @brief Set up simulation rules
 * @param ac Argument count (not including program name)
 * @param av Arguments (not including program name)
 * @param rules The rules struct
 * 
 * @return
 * Return 0 if malloc failed. Else return 1.
*/
/**
 * @attention For rules->iteration, if it's 0, meaning the iteration mode
 *            is not enabled. If it's greater than 0, iteration mode is
 * 			  enabled, should check how many each philo has eaten.
*/
static void	pl_setup_rules(int ac, char **av, t_rules *rules)
{
	rules->sim_state = RUN;
	rules->philo_total = ft_atoi(*av++);
	rules->time_to_die = ft_atoi(*av++);
	rules->time_to_eat = ft_atoi(*av++);
	rules->time_to_sleep = ft_atoi(*av++);
	if (ac == 5)
		rules->iteration = ft_atoi(*av);
	else
		rules->iteration = 0;
	rules->philo_full = 0;
	if (pl_lock_setup(&rules->locks, SHARED) == 0)
		return ;
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
