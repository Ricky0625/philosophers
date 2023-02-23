/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pl_philo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wricky-t <wricky-t@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 14:23:17 by wricky-t          #+#    #+#             */
/*   Updated: 2023/02/23 17:59:40 by wricky-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/**
 * @brief Initialize each philo and assign them their forks
 * @param sim The simulation struct
 * @param philo The struct of each individual philo
 * @param id The id of the philo. Noted that when initialized each philo,
 *           the id starts with 0. The subject asked us to display the philo
 *           ID from 1. Hence when printing it, increment one.
 */
int	pl_philo_init(t_simulation *sim, t_philo *philo, int id)
{
	philo->id = id;
	philo->meal_count = 0;
	philo->full = NOTFULL;
	philo->last_ate = pl_get_time();
	philo->rules = sim->rules;
	if (pl_lock_setup(NULL, philo, PHILO) == 0)
		return (0);
	return (1);
}

/**
 * @brief Spawn philo. Create process for each philo. Spawn one monitor to
 * 	      each philo as well. The monitor thread will be detached, run in
 *        background.
 * @param sim The simulation struct
 *
 * @return 1 if all the philo can be initialized and philos are running.
 *         0 if there's an issue when creating a thread, process.
 */
int	pl_spawn_philo(t_simulation *sim)
{
	int			i;
	pid_t		pid;
	t_philo		*philos;
	pthread_t	monitor;

	i = -1;
	philos = sim->philos;
	while (++i < sim->rules->philo_total)
	{
		if (pl_philo_init(sim, &philos[i], i) == 0)
			return (0);
		pid = fork();
		if (pid < 0)
			return (pl_show_error(FORK_FAILED, -1));
		if (pid == 0)
		{
			if (pthread_create(&monitor, NULL, &pl_monitor,
					(void *)&philos[i]) != 0)
				return (pl_show_error(CREATE_THD_FAILED, i));
			pthread_detach(monitor);
			pl_routine(&philos[i]);
			break ;
		}
	}
	return (1);
}

/**
 * @brief Initialize the simulation environment
 * @param sim The simulation struct
 * @param rules The rules struct
 *
 * 1. Set rules
 * 2. Prepare forks for philosophers (semaphore)
 * 3. Set start time
 * 4. Spawn the philosophers using fork()
 */
int	pl_simulation_init(t_simulation *sim, t_rules *rules)
{
	sim->rules = rules;
	if (pl_sem_open(&sim->forks, FORK_SEM, rules->philo_total) == 0)
		return (0);
	rules->forks = sim->forks;
	sim->philos = malloc(sizeof(t_philo) * rules->philo_total);
	if (sim->philos == NULL)
		return (0);
	rules->start_time = pl_get_time();
	pl_spawn_philo(sim);
	return (1);
}

/**
 * @brief Begin the simulation based on the rules
 * @param rules The simulation rules
 *
 * @details
 * 1. Initialize the simulation
 * 2. Wait for each philo to exit
 * 3. Unlink and close the semaphores
 */
void	pl_begin_simulation(t_rules *rules)
{
	int				i;
	t_simulation	*sim;

	sim = malloc(sizeof(t_simulation));
	if (sim == NULL)
		return ;
	pl_simulation_init(sim, rules);
	i = -1;
	while (++i < rules->philo_total)
		waitpid(-1, NULL, 0);
	sem_close(sim->forks);
	sem_unlink(FORK_SEM);
}
