/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pl_monitor_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wricky-t <wricky-t@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 11:12:24 by wricky-t          #+#    #+#             */
/*   Updated: 2023/02/23 17:51:52 by wricky-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/**
 * @brief Get assigned philo's last ate time
 * @param philo The assigned philo
 *
 * @details
 * Last ate is a variable that will be accessed by both monitor and philo.
 * Philo will update (write) is when he is eating while monitor will use
 * it to check (read) if a philo to starve to dead. Hence to prevent this
 * variable being accessed by both monitor and philo, use semaphore 
 * to protect the critical section.
 *
 * @return
 * Philo's last ate time
 */
time_t	pl_get_last_ate(t_philo *philo)
{
	time_t	last_ate;

	sem_wait(philo->last_ate_sem);
	last_ate = philo->last_ate;
	sem_post(philo->last_ate_sem);
	return (last_ate);
}

/**
 * @brief Get the simulation's state
 * @param philo The assigned philo
 *
 * @details
 * Sim state (simulation state) will also be accessed by both monitor and philo.
 * But this time, monitor is the one that will update it; philo will only
 * read the value of it and react to it. Hence, a semaphore is required to
 * protect the critical section.
 *
 * @return
 * Simulation state
 */
t_state	pl_get_sim_state(t_philo *philo)
{
	t_state	sim_state;

	sem_wait(philo->rules->locks.sim_state_sem);
	sim_state = philo->rules->sim_state;
	sem_post(philo->rules->locks.sim_state_sem);
	return (sim_state);
}

/**
 * @brief Get philo's meal count
 * @param philo The assigned philo
 *
 * @details
 * Meal count will be accessed by both monitor and philo as well. Philo
 * will update the variable and monitor will read it. A semaphore is
 * required to prevent both of them access the value at the same time.
 *
 * @return
 * Meal count
 */
int	pl_get_meal_count(t_philo *philo)
{
	int	meal_count;

	sem_wait(philo->meal_sem);
	meal_count = philo->meal_count;
	sem_post(philo->meal_sem);
	return (meal_count);
}
