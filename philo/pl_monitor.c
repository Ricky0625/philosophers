/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pl_monitor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wricky-t <wricky-t@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 15:34:46 by wricky-t          #+#    #+#             */
/*   Updated: 2023/02/20 18:09:39 by wricky-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Keep track if the assigned philo is full and also to calculate
 *        the total number philo that is full.
 * @param philo the assigned philo
 * 
 * @details
 * This function will only do calculation if the simulation rules requires
 * the philosopher to eat at least how many meal.
 * The function checks if the assigned philosopher has eaten what's required.
 * If yes and the assigned philo has not "declared" as full yet, increment
 * philo_full, which is use to keep track how many philosopher is full.
 * The second half of this function is to check if the number of philosopher
 * that is full reach the total number of philosopher. If yes, set
 * simulation state to "END" to indicate that the simulation shall end now.
 * 
 * @return
 * 0, If the optional rules is not set or not all philo is full
 * 1, If all the philo is full.
*/
int	pl_full_tracker(t_philo *philo)
{
	int	iteration;

	if (philo->rules->iteration == 0)
		return (0);
	pthread_mutex_lock(&philo->rules->locks.full_lock);
	iteration = philo->rules->iteration;
	if (pl_get_meal_count(philo) >= iteration && philo->full == NOTFULL)
	{
		philo->rules->philo_full++;
		philo->full = FULL;
	}
	if (philo->rules->philo_full == philo->rules->philo_total)
	{
		pl_fork_action(philo, RETURN);
		pthread_mutex_lock(&philo->rules->locks.sim_state_lock);
		philo->rules->sim_state = END;
		pthread_mutex_unlock(&philo->rules->locks.sim_state_lock);
		pthread_mutex_unlock(&philo->rules->locks.full_lock);
		return (1);
	}
	pthread_mutex_unlock(&philo->rules->locks.full_lock);
	return (0);
}

/**
 * @brief Check if a philo will die of starvation or not
 * @param philo The assigned philo
 * 
 * @details
 * This function checks if assigned philo will die of starvation or not.
 * This is done by comparing the philo's last ate time and the time to die.
 * When the compared value is greater and equal than the time to die,
 * set the simulation state to "END". Declare the assign philo as dead.
 * 
 * @return
 * 1, if the assigned philo died because of starvation.
 * 0, if the assigned philo did not died.
*/
int	pl_check_dead(t_philo *philo)
{
	time_t	curr_time;
	time_t	last_ate;

	pthread_mutex_lock(&philo->rules->locks.death_lock);
	last_ate = pl_get_last_ate(philo);
	curr_time = pl_get_time();
	if ((curr_time - last_ate) > philo->rules->time_to_die)
	{
		pl_declare_state(philo, DIED);
		pl_fork_action(philo, RETURN);
		pthread_mutex_lock(&philo->rules->locks.sim_state_lock);
		philo->rules->sim_state = END;
		pthread_mutex_unlock(&philo->rules->locks.sim_state_lock);
		pthread_mutex_unlock(&philo->rules->locks.death_lock);
		return (1);
	}
	pthread_mutex_unlock(&philo->rules->locks.death_lock);
	return (0);
}

/**
 * @brief Routine for monitor thread
 * @param arg The philo struct
 * 
 * @details
 * The idea of this routine is to check if the philosopher will die because
 * of starvation. As well as if all the philo are full or not.
 * 
 * Same as philosophers' routine, when the simulation state is END, end the
 * routine.
 * 
 * TODO: Ask Joseph why usleep is necessary.
*/
void	*pl_monitor(void *arg)
{
	t_philo	*philo;

	philo = arg;
	while (1)
	{
		if (pl_get_sim_state(philo) == END)
			break ;
		if (pl_check_dead(philo) == 1 || pl_full_tracker(philo) == 1)
			break ;
		usleep(philo->rules->time_to_die * 1000 / 2);
	}
	return (NULL);
}
