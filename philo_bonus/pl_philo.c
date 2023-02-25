/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pl_philo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wricky-t <wricky-t@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 11:17:36 by wricky-t          #+#    #+#             */
/*   Updated: 2023/02/25 16:43:08 by wricky-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/**
 * @brief Initialize philo and set the semaphore for shared data
 * @param sim The simulation struct
 * @param philo the philosopher struct
 * @param id The id
*/
int	pl_philo_init(t_simulation *sim, t_philo *philo, int id)
{
	philo->id = id;
	philo->meal_count = 0;
	philo->full = NOTFULL;
	philo->last_ate = pl_get_time();
	philo->rules = sim->rules;
	if (pl_setup_philo_sem(philo) == 0)
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
 * 
 * @attention
 * When the fork is successful, fork will return 0 to indicate that
 * child has been created successfully and the child is running. The
 * pid of a child is not actually 0.
 */
int	pl_spawn_philo(t_simulation *sim)
{
	int		i;
	pid_t	pid;
	t_philo	*philos;

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
			pl_routine(&philos[i]);
			break ;
		}
		sim->rules->pids[i] = pid;
	}
	return (1);
}

/**
 * @brief Initialize the simulation environment
 * @param sim The simulation struct
 * @param rules The rules struct
 * 
 * 1. Set rules
 * 2. Open semaphore for the forks
 * 3. Malloc an array for pids (store pid of child)
 * 4. Get start time
 * 5. Set shared semaphores
 * 6. Spawn philo
 * 7. Check if the philo is full (if option is enabled)
*/
int	pl_simulation_init(t_simulation *sim, t_rules *rules)
{
	sim->rules = rules;
	if (pl_sem_open(&rules->forks, FORK_SEM, rules->philo_total) == 0)
		return (0);
	sim->philos = malloc(sizeof(t_philo) * rules->philo_total);
	if (sim->philos == NULL)
		return (0);
	rules->pids = malloc(sizeof(pid_t) * rules->philo_total);
	if (rules->pids == NULL)
		return (0);
	rules->start_time = pl_get_time();
	if (pl_setup_shared_sem(&rules->locks) == 0)
		return (0);
	if (pl_spawn_philo(sim) == 0)
		return (0);
	pl_check_full(rules);
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
	t_simulation	*sim;

	sim = malloc(sizeof(t_simulation));
	if (sim == NULL)
		return ;
	if (pl_simulation_init(sim, rules) == 0)
		return ;
	while (waitpid(-1, NULL, 0) != -1)
		;
}
