/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pl_routine.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wricky-t <wricky-t@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 16:26:09 by wricky-t          #+#    #+#             */
/*   Updated: 2023/02/16 14:15:42 by wricky-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Function to handle mutex (fork action)
 * @param philo The philo struct
 * @param act Fork action. take: Lock mutex; return: Unlock mutex.
*/
void	pl_fork_action(t_philo *philo, t_fork_action act)
{
	if (act == TAKE)
	{
		pthread_mutex_lock(philo->left_fork);
		pl_declare_state(philo, FORK);
		pthread_mutex_lock(philo->right_fork);
		pl_declare_state(philo, FORK);
	}
	else if (act == RETURN)
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
	}
}

/**
 * @brief Philo THINK
 * 
 * @details
 * The process to take the forks. When philo has taken two forks, start eating.
*/
void	pl_think(t_philo *philo)
{
	// get timestamp here
	pl_declare_state(philo, THINK);
}

void	pl_eat(t_philo *philo)
{
	pl_fork_action(philo, TAKE);
	philo->last_ate = pl_get_time(); // need to lock this
	pl_declare_state(philo, EAT);
	philo->meal_count++; // need to lock this
	usleep(philo->rules->time_to_eat * 1000);
	pl_fork_action(philo, RETURN);
}

void	pl_sleep(t_philo *philo)
{
	pl_declare_state(philo, SLEEP);
	usleep(philo->rules->time_to_sleep * 1000);
}

/**
 * @brief The routine of each philo
*/
/**
 * 1. Take forks
 * 2. Eat
 * 3. Sleep
 * 4. Think
*/
void	*pl_routine(void *arg)
{
	t_philo	*philo;

	philo = arg;
	if (philo->id % 2 != 0)
		pl_usleep(philo->rules->time_to_eat);
	while (1)
	{
		// exit condition of each philo should place here
		pl_eat(philo);
		pl_sleep(philo);
		pl_think(philo);
	}
	return (NULL);
}
