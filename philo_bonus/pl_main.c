/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pl_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wricky-t <wricky-t@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/10 15:14:32 by wricky-t          #+#    #+#             */
/*   Updated: 2023/02/23 11:32:56 by wricky-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/**
 * @brief Get current time in seconds
 *
 * @return Time in time_t format
 */
time_t	pl_get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

/**
 * @brief A better usleep
 *
 * @details
 * Through some experiments as well as peer's observation, it seems like
 * usleep is not that accurate in some case. Usleep might "overslept", which
 * makes the timestamp of the program less accurate. Hence, to prevent this
 * situation, the idea is to let usleep to sleep a constant amount of time
 * and check if the program has slept for what's required.
 */
void	pl_usleep(time_t sec)
{
	time_t	cur_time;

	cur_time = pl_get_time();
	while (1)
	{
		usleep(500);
		if ((pl_get_time() - cur_time) >= sec)
			break ;
	}
}

/**
 * @brief The main function
 * 
 * 1. Parse the inputs and set the rules
 * 2. Begin the simulation based on the rules set
*/
int	main(int ac, char **av)
{
	t_rules	rules;

	if (pl_parse(--ac, ++av, &rules) == 0)
		return (EXIT_FAILURE);
	pl_begin_simulation(&rules);
	return (EXIT_SUCCESS);
}
