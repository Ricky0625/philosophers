/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pl_parse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wricky-t <wricky-t@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 10:45:21 by wricky-t          #+#    #+#             */
/*   Updated: 2023/02/22 14:26:09 by wricky-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/**
 * @brief Setup additional locks that prevent data race
 * @param locks The locks struct
 * @param philo The philo struct
 * @param type The lock type (SHARED / PHILO)
 *
 * @details
 * 1. Last ate lock - Philo's last ate time. (PHILO)
 * 2. Meal count lock - Philo's meal count. (PHILO)
 * 3. Declare lock - Philo's state declaration. (SHARED)
 * 4. Sim state lock - Simulation's state. (SHARED)
 * 5. Death lock - Check if dead process lock. (SHARED)
 * 6. Full lock - Check if full process lock. (SHARED)
 *
 * @return
 * 1, If all additional locks can be initialized.
 * 0, If there's an error while creating a mutex.
 */
/**
 * TODO: Do I need this?
*/
int	pl_lock_setup(t_locks *locks, t_philo *philo, t_lock_type type)
{
	if (type != SHARED && type != PHILO)
		return (0);
	if (type == SHARED)
	{
		if (pthread_mutex_init(&locks->declare_lock, NULL) != 0)
			return (pl_show_error(CREATE_MUT_FAILED, -1));
		if (pthread_mutex_init(&locks->sim_state_lock, NULL) != 0)
			return (pl_show_error(CREATE_MUT_FAILED, -1));
		if (pthread_mutex_init(&locks->death_lock, NULL) != 0)
			return (pl_show_error(CREATE_MUT_FAILED, -1));
		if (pthread_mutex_init(&locks->full_lock, NULL) != 0)
			return (pl_show_error(CREATE_MUT_FAILED, -1));
		return (1);
	}
	if (pthread_mutex_init(&philo->last_ate_lock, NULL) != 0)
		return (pl_show_error(CREATE_MUT_FAILED, -1));
	if (pthread_mutex_init(&philo->meal_count_lock, NULL) != 0)
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
/**
 * TODO: Might need slight modification on this function
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
	if (pl_lock_setup(&rules->locks, NULL, SHARED) == 0)
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
