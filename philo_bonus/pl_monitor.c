/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pl_monitor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wricky-t <wricky-t@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 16:39:39 by wricky-t          #+#    #+#             */
/*   Updated: 2023/02/25 16:40:30 by wricky-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/**
 * @brief Kill all the philos at once
 * @param rules The rule struct
*/
void	pl_kill_philos(t_rules *rules)
{
	int	i;

	i = -1;
	while (++i < rules->philo_total)
		kill(rules->pids[i], SIGINT);
}

/**
 * @brief To check if all the philos are full
 * @param rules the simulation rules
 * 
 * @details
 * By default the full_sem is 0. When a philo is full, it will do a
 * sem_post to increment the value of semaphore by 1. Inside this function,
 * there's one while loop that will sem_wait for "philo_total" times.
 * As mentioned, initial value of full_sem is 0, and when we do a sem_wait
 * on a empty semaphore, it will wait until the value of semaphore is not
 * longer 0 then decrement the value by 1.
 * By following this logic, this is why the while loop will loop do sem_post
 * for "philo_total" times and able to check if all philos are full.
 * When all philos are full, sem_wait the declare sem. This is to prevent
 * any philo to declare their state.
 * Then, kill all philos!!!!!!
*/
void	pl_check_full(t_rules *rules)
{
	int		i;

	i = -1;
	if (rules->iteration == 0)
		return ;
	while (++i < rules->philo_total)
		sem_wait(rules->locks.full_sem);
	sem_wait(rules->locks.declare_sem);
	pl_kill_philos(rules);
}

/**
 * @brief Monitor the status of each philo
 * @param arg Assigned philo
 * 
 * @details
 * Check if the philo to starve to death. When that happens, declare
 * that the assigned philo is dead and sem_wait declare_sem to
 * prevent other philo to declare their state. Kill all philos afterward.
*/
void	*pl_monitor(void *arg)
{
	t_philo	*philo;
	time_t	last_ate;

	philo = arg;
	while (1)
	{
		sem_wait(philo->last_ate_sem);
		last_ate = philo->last_ate;
		sem_post(philo->last_ate_sem);
		if ((pl_get_time() - last_ate) > philo->rules->time_to_die)
		{
			pl_declare_state(philo, DIED);
			sem_wait(philo->rules->locks.declare_sem);
			break ;
		}
	}
	pl_kill_philos(philo->rules);
	return (NULL);
}
