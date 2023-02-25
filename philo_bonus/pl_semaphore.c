/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pl_semaphore.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wricky-t <wricky-t@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 11:29:12 by wricky-t          #+#    #+#             */
/*   Updated: 2023/02/25 14:26:00 by wricky-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/**
 * @brief Basically sem_open
 * @param sem Pointer to the semaphore
 * @param name The name of the semaphore
 * @param value Initial value of the semaphore
 * 
 * @details
 * All semaphore will be granted with this premission, 0644.
 * 644 meaning:
 * 	  6 - File owner can read and write
 *    4 - Users in the same group as owner can read
 *    4 - All users can read
 * 
 * @attention
 * Calling sem_unlink before sem_open is to ensure that any
 * existing semphore with the same name is removed before
 * creating a new one.
*/
int	pl_sem_open(sem_t **sem, char *name, int value)
{
	sem_unlink(name);
	*sem = sem_open(name, O_CREAT, 0644, value);
	if (*sem == SEM_FAILED)
		return (pl_show_error(OPEN_SEM_FAILED, 0));
	return (1);
}

/**
 * @brief Open semaphore for all the shared sempahores
 * @param locks The struct that store all the shared semaphores
 * 
 * @details
 * DECLARE_SEM - use to declare the philo's state
 * SIM_SEM - use to check if the simulation end
 * FULL_SEM - check how many philo are full
 * 
 * @attention
 * Shall call in parent so that the semaphores could be shared
 * among the child process.
 * 
 * @return
 * 0, if there's an error
 * 1, if everything is a success
*/
int	pl_setup_shared_sem(t_locks *locks)
{
	if (pl_sem_open(&locks->declare_sem, DECLARE_SEM, 1) == 0)
		return (0);
	if (pl_sem_open(&locks->sim_sem, SIM_SEM, 0) == 0)
		return (0);
	if (pl_sem_open(&locks->full_sem, FULL_SEM, 0) == 0)
		return (0);
	return (1);
}

/**
 * @brief Setup semaphore that is private to philo and its
 *        monitor thread.
 * @param philo The philo struct
 * 
 * @attention
 * The name of the semaphore is unique for each philo to
 * prevent every philo accidentally use the same one.
*/
int	pl_setup_philo_sem(t_philo *philo)
{
	char	*id;
	char	*sem_name;

	id = ft_itoa(philo->id + 1);
	sem_name = ft_strjoin(LAST_ATE_SEM, id);
	free(id);
	if (pl_sem_open(&philo->last_ate_sem, sem_name, 1) == 0)
		return (0);
	free(sem_name);
	return (1);
}
