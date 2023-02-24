/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pl_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wricky-t <wricky-t@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 10:46:32 by wricky-t          #+#    #+#             */
/*   Updated: 2023/02/24 11:08:15 by wricky-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	main(int ac, char **av)
{
	t_rules	rules;

	if (pl_parse(--ac, ++av, &rules) == 0)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
