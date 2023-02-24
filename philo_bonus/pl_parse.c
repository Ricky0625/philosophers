/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pl_parse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wricky-t <wricky-t@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 11:08:38 by wricky-t          #+#    #+#             */
/*   Updated: 2023/02/24 11:10:09 by wricky-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static int	pl_check_args(int ac, char **av)
{
	int	i;
	int	arg;

	i = -1;
	while (++i < ac)
	{
		if (ft_isdigit_str(av[i]) == 0)
			return (pl_show_error(NON_NUMERIC_ARGS, 0));
		arg = ft_atoi(av[i]);
		if (arg < 0)
			return (pl_show_error(NEGATIVE_ARGS, 0));
		if (arg == 0 && i == 4)
			return (pl_show_error(INVALID_OPTION, 0));
	}
	return (1);
}

int pl_parse(int ac, char **av, t_rules *rules)
{
    if (ac != 4 && ac != 5)
        return (pl_show_error(INVALID_ARGS_TOTAL, 0));
    if (pl_check_args(ac, av) == 0)
        return (0);
    // setup rules
    (void)rules;
    return (1);
}
