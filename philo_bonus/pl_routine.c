/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pl_routine.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wricky-t <wricky-t@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 14:59:58 by wricky-t          #+#    #+#             */
/*   Updated: 2023/02/25 16:44:12 by wricky-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/**
 * @brief Function to let philo take forks
 * @param philo The philo struct
 * @param act Fork action.
 * 
 * @details
 * If act is "TAKE", sem_wait. (Take fork)
 * If act is "RETURN", sem_post. (Return fork)
*/
void	pl_fork_action(t_philo *philo, t_fork_action act)
{
	if (act == TAKE)
	{
		sem_wait(philo->rules->forks);
		pl_declare_state(philo, FORK);
		sem_wait(philo->rules->forks);
		pl_declare_state(philo, FORK);
	}
	else if (act == RETURN)
	{
		sem_post(philo->rules->forks);
		sem_post(philo->rules->forks);
	}
}

/**
 * @brief Eat routine of philo
 * @param philo The assigned philo
 * 
 * @details
 * Routine:
 * 1. Take forks (Left & Right)
 * 2. Update last_ate time before eating (need to protect)
 * 3. Declare EAT as the assigned philo's state
 * 4. Update the meal count; (need to protect)
 * 5. Check if the phil is full, if yes, sem_post full_sem
 * 6. Put down forks (Left & Right)
*/
void	pl_eat(t_philo *philo)
{
	pl_fork_action(philo, TAKE);
	sem_wait(philo->last_ate_sem);
	philo->last_ate = pl_get_time();
	sem_post(philo->last_ate_sem);
	pl_declare_state(philo, EAT);
	philo->meal_count++;
	if (philo->meal_count == philo->rules->iteration)
		sem_post(philo->rules->locks.full_sem);
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
 * The idea is to let philo do its own thing. When time comes,
 * either is someone died or everyone is full, parent process
 * will kill each of them. Hehe.
*/
void	pl_routine(t_philo *philo)
{
	pthread_t	monitor;

	if (philo->id % 2 != 0)
		pl_usleep(philo->rules->time_to_eat / 2);
	if (pthread_create(&monitor, NULL, &pl_monitor, (void *)philo) != 0)
	{
		pl_show_error(CREATE_THD_FAILED, philo->id);
		return ;
	}
	pthread_detach(monitor);
	while (1)
	{
		pl_eat(philo);
		pl_sleep(philo);
		pl_declare_state(philo, THINK);
	}
}
