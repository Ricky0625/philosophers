/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pl_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wricky-t <wricky-t@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 10:46:32 by wricky-t          #+#    #+#             */
/*   Updated: 2023/02/25 17:11:42 by wricky-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/**
 * Something that worth knowing:
 * 1. When you open a semaphore in parent process, the semaphore is shared
 *    among all child. Let's say if you have 10 child and a semaphore that has
 * 	  an initial value of 5. Try call sem_wait() and print "Take one!" in your
 *    child process, and you should see the message 5 times on STDOUT.
 * 2. When you create a thread in a child process, the thread will share the
 *    resource with the child process instead, not the parent. This is because
 *    child process has its own data segment, memory space, etc.
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
