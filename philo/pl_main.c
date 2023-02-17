/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pl_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wricky-t <wricky-t@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/10 15:14:32 by wricky-t          #+#    #+#             */
/*   Updated: 2023/02/15 15:34:10 by wricky-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * TODO: Create thread for each philo to monitor them
 * 1. Based on Ray's submission, create just one thread to monitor has
 *    proven to be slow.
 * 2. The monitor thread should check if philo will die because of starvation
 *    and also set the program to exit (need to figure out how to do this)
 * 3. Idea is to let the monitor threads to run another routine in background
 *    (detach)
*/

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
