/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pl_philo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wricky-t <wricky-t@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 14:23:17 by wricky-t          #+#    #+#             */
/*   Updated: 2023/02/22 11:39:59 by wricky-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Initialize each philo and assign them their forks
 * @param sim The simulation struct
 * @param philo The struct of each individual philo
 * @param id The id of the philo. Noted that when initialized each philo,
 *           the id starts with 0. The subject asked us to display the philo
 *           ID from 1. Hence when printing it, increment one.
 * 
 * @details
 * The forks will be assigned in this way:
 * 1. Left fork: Own index's fork (mutex)
 * 2. Right fork: Own index's -1 fork (mutex). If the id is 0, meaning that
 *    philo should grab the fork at [total philo - 1].
 * The idea is to let the philo know that when they need to eat, they have
 * to use the fork assigned to them. Of course, need to check if the fork
 * is taken or not.
*/
int	pl_philo_init(t_simulation *sim, t_philo *philo, int id)
{
	philo->id = id;
	philo->meal_count = 0;
	philo->full = NOTFULL;
	philo->last_ate = pl_get_time();
	philo->left_fork = &sim->forks[id];
	philo->rules = sim->rules;
	if (pl_lock_setup(NULL, philo, PHILO) == 0)
		return (0);
	if (id == 0)
		philo->right_fork = &sim->forks[sim->rules->philo_total - 1];
	else
		philo->right_fork = &sim->forks[id - 1];
	return (1);
}

/**
 * @brief Preparing forks for philosophers
 * @param sim The simulation struct
 * 
 * @return 1 if mutexes can be initialized without error. 0 if one of the mutex
 *         cannot be initialized.
 */
int	pl_prepare_forks(t_simulation *sim)
{
	int				i;
	pthread_mutex_t	*forks;

	i = -1;
	forks = sim->forks;
	while (++i < sim->rules->philo_total)
	{
		if (pthread_mutex_init(&forks[i], NULL) != 0)
			return (pl_show_error(CREATE_MUT_FAILED, i));
	}
	return (1);
}

/**
 * @brief Spawn philo. Create thread for each philo. Spawn one monitor to
 * 	      each philo as well. The monitor thread will be detached, run in
 *        background.
 * @param sim The simulation struct
 * 
 * @return 1 if all the philo can be initialized and thread has started.
 *         0 if there's an issue when creating a thread.
*/
int	pl_spawn_philo(t_simulation *sim)
{
	int			i;
	t_philo		*pl;
	pthread_t	monitor;

	i = -1;
	pl = sim->philos;
	while (++i < sim->rules->philo_total)
	{
		if (pl_philo_init(sim, &pl[i], i) == 0)
			return (0);
		if (pthread_create(&pl[i].me, NULL, &pl_routine, (void *)&pl[i]) != 0)
			return (pl_show_error(CREATE_THD_FAILED, i));
		if (pthread_create(&monitor, NULL, &pl_monitor, (void *)&pl[i]) != 0)
			return (pl_show_error(CREATE_THD_FAILED, i));
		pthread_detach(monitor);
	}
	return (1);
}

/**
 * @brief Initialize the simulation environment
 * @param sim The simulation struct
 * @param rules The rules struct
 * 
 * 1. Get the simulation start time. Will use this to get timestamp of
 * 	  philo's activity.
 * 2. If the "iteration" option is enabled. Use this to check how
 *    many philo is full.
 * 3. Set rules
 * 4. Prepare forks for philosophers
 * 5. Spawn the philosophers
*/
int	pl_simulation_init(t_simulation *sim, t_rules *rules)
{
	sim->rules = rules;
	sim->forks = malloc(sizeof(pthread_mutex_t) * rules->philo_total);
	if (sim->forks == NULL)
		return (0);
	sim->philos = malloc(sizeof(t_philo) * rules->philo_total);
	if (sim->philos == NULL)
		return (0);
	if (pl_prepare_forks(sim) == 0)
		return (0);
	rules->start_time = pl_get_time();
	if (pl_spawn_philo(sim) == 0)
		return (0);
	return (1);
}

/**
 * @brief Begin the simulation based on the rules
 * @param rules The simulation rules
 * 
 * @details
 * 1. Initialize the simulation
 * 2. Destroy forks and philo at the end
*/
void	pl_begin_simulation(t_rules *rules)
{
	int				i;
	t_simulation	*sim;

	sim = malloc(sizeof(t_simulation));
	pl_simulation_init(sim, rules);
	i = -1;
	while (++i < rules->philo_total)
	{
		if (pthread_join(sim->philos[i].me, NULL) != 0)
			break ;
	}
	free(sim->forks);
	free(sim->philos);
	free(sim);
}
