/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pl_monitor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wricky-t <wricky-t@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 15:34:46 by wricky-t          #+#    #+#             */
/*   Updated: 2023/02/16 13:35:39 by wricky-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Keep track if the assigned philo is full and also to calculate
 *        the total number philo that is full.
 * @param philo the assigned philo
*/
int	pl_full_tracker(t_philo *philo)
{
	// might have to reconsider this
	if (philo->rules->iteration == 0)
		return (0);
	// check individual philo if they are full or not
	if (philo->meal_count >= philo->rules->iteration && philo->full == 0)
	{
		philo->rules->philo_full++; // need to unlock and lock this
		philo->full = 1;
	}
	// check if all philo already full or not
	if (philo->rules->philo_full == philo->rules->philo_total)
	{
		// need to lock and unlock philo_full
		
		// current idea is to have a variable that can be set here
		// in philo's routine (while loop), check if that value is the exit
		// condition
		return (1);
	}
	return (0);
}

/**
 * @brief Check if a philo will die of starvation or not
 * @param philo The assigned philo
*/
int	pl_check_dead(t_philo *philo)
{
	time_t	curr_time;

	if (philo->last_ate == 0)
		return (0);
	curr_time = pl_get_time();
	// might need to lock and unlock last ate
	if ((curr_time - philo->last_ate) >= philo->rules->time_to_die)
	{
		// need to set something that stops all philos and monitors
		pl_declare_state(philo, DIED);
		return (1);
	}
	return (0);
}

/**
 * @brief Routine for monitor thread
 * @param arg The philo struct
 * 
 * @details
 * The idea of this routine is to check if the philosopher will die because
 * of starvation.
*/
/**
 * The idea here is monitor thread will check the assigned philo, if either one condition:
 * 1. the assigned philo dead
 * 2. the assigned philo is full and same as others
 * ... this function will then break.
 * When this function breaks, it means the monitor thread has done its job.
 * When either one condition fulfilled, monitor thread should set something that will
 * cause all philo to stop its routine.
*/
/**
 * TODO:
 * Need a way to let other monitor thread to stop checking as well.
*/
void	*pl_monitor(void *arg)
{
	t_philo	*philo;

	philo = arg;
	while (1)
	{
		// the exit condition should put here
		if (pl_check_dead(philo) == 1)
			break ;
		if (pl_full_tracker(philo) == 1)
			break ;
	}
	return (NULL);
}
