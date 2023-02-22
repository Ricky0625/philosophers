/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pl_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wricky-t <wricky-t@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/10 15:14:32 by wricky-t          #+#    #+#             */
/*   Updated: 2023/02/22 12:12:57 by wricky-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
