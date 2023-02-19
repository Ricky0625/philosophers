/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pl_routine.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wricky-t <wricky-t@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 16:26:09 by wricky-t          #+#    #+#             */
/*   Updated: 2023/02/19 16:53:32 by wricky-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Function to handle mutex (fork action)
 * @param philo The philo struct
 * @param act Fork action. take: Lock mutex; return: Unlock mutex.
 * 
 * @details
 * If act is "TAKE", lock mutexes.
 * If act is "RETURN", unlock mutexes.
 * 
 * "Mutexes" here are referring to the left and right fork.
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
 * @brief Eat routine of philo
 * @param philo The assigned philo
 * 
 * @details
 * Routine:
 * 1. Take forks (Left & Right)
 * 2. Update last_ate time before eating
 * 3. Declare EAT as the assigned philo's state
 * 4. Update the meal count;
 * 5. Eat for "time_to_eat"
 * 6. Put down forks (Left & Right)
*/
void	pl_eat(t_philo *philo)
{
	pl_fork_action(philo, TAKE);
	pthread_mutex_lock(&philo->rules->locks.last_ate_lock);
	philo->last_ate = pl_get_time();
	pthread_mutex_unlock(&philo->rules->locks.last_ate_lock);
	pl_declare_state(philo, EAT);
	pthread_mutex_lock(&philo->rules->locks.meal_count_lock);
	philo->meal_count++;
	pthread_mutex_unlock(&philo->rules->locks.meal_count_lock);
	pl_usleep(philo->rules->time_to_eat);
	pl_fork_action(philo, RETURN);
}

/**
 * @brief Sleep routine of philo
 * 
 * Declare state and sleep for "time_to_sleep"
*/
void	pl_sleep(t_philo *philo)
{
	pl_declare_state(philo, SLEEP);
	pl_usleep(philo->rules->time_to_sleep);
}

/**
 * @brief The routine of each philo
 * @param arg The assigned philo
 * 
 * @details
 * Every philosophers need to take turns to have their meal.
 * This is because if everyone takes the fork at the same time,
 * it will cause a deathlock and everyone will wait till died.
 * Since when one philo needs to eat, he needs to take the fork
 * of his and the one sitting next to him. So, the idea here is
 * to seperate the philosophers into two big group. If the 
 * ID of the philo is odd number, they have to wait for
 * "time_to_eat" time. Else, they can perform the routine first.
 * 
 * Routine:
 * 1. Eat
 * 2. Sleep
 * 3. Think
 * 
 * When the state of the simulation is set to "END", every
 * philo shall stop his routine and join the thread back
 * to main thread.
 * 
 * @return
 * Always return a (void *)NULL. The return value is useless.
*/
void	*pl_routine(void *arg)
{
	t_philo	*philo;

	philo = arg;
	if (philo->id % 2 != 0)
		pl_usleep(philo->rules->time_to_eat / 2);
	while (1)
	{
		if (pl_get_sim_state(philo) == END)
			break ;
		pl_eat(philo);
		pl_sleep(philo);
		pl_declare_state(philo, THINK);
	}
	return (NULL);
}
