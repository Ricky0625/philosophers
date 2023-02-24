/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pl_msg.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wricky-t <wricky-t@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 11:12:39 by wricky-t          #+#    #+#             */
/*   Updated: 2023/02/24 11:12:56 by wricky-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/**
 * @brief Show error message
 * @param error Error type
 * @param id The id of the philo
 * 
 * @return
 * 1, if the error type is unknown
 * 0, if there's an error
*/
int	pl_show_error(t_error error, int id)
{
	if (error < 0 || error >= ERROR_TOTAL)
		return (1);
	printf(RED"[ERROR]: ");
	if (error == INVALID_ARGS_TOTAL)
		printf("Require at least 4 or 5 arguments!\n\n");
	else if (error == NON_NUMERIC_ARGS)
		printf("Found non numeric arguments!\n\n");
	else if (error == NEGATIVE_ARGS)
		printf("Found Negative numeric argument!\n\n");
	else if (error == INVALID_OPTION)
		printf("Optional argument should more than 0!\n\n");
	else if (error == CREATE_THD_FAILED)
		printf("Failed to create PHILO %d\n", id);
	else if (error == OPEN_SEM_FAILED)
		printf("Failed to open SEMAPHORE\n");
	else if (error == FORK_FAILED)
		printf("Failed to FORK child!\n");
	if (!(error >= CREATE_THD_FAILED && error < ERROR_TOTAL))
		printf(GRN"Run `make help` for help.\n"DEF);
	return (0);
}
