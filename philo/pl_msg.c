/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pl_msg.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wricky-t <wricky-t@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 10:13:17 by wricky-t          #+#    #+#             */
/*   Updated: 2023/02/16 13:43:21 by wricky-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	pl_show_error(t_error error, int id)
{
	if (error < 0 || error >= ERROR_TOTAL)
		return ;
	printf(RED"[ERROR]: ");
	if (error == INVALID_ARGS_TOTAL)
		printf("Require at least 4 or 5 arguments!\n\n");
	else if (error == NON_NUMERIC_ARGS)
		printf("Found non numeric arguments!\n\n");
	else if (error == NEGATIVE_ARGS)
		printf("Found Negative numeric argument!\n\n");
	else if (error == CREATE_THD_FAILED)
		printf("Failed to create PHILO %d\n", id);
	else if (error == CREATE_MUT_FAILED)
		printf("Failed to create MUTEX %d\n", id);
	else if (error == DESTROY_MUT_FAILED)
		printf("Failed to destroy MUTEX %d\n", id);
	if (!(error >= CREATE_THD_FAILED && error < ERROR_TOTAL))
		printf(GRN"Run `make help` for help.\n"DEF);
}

void	pl_declare_state(t_philo *philo, t_state state)
{
	time_t	curr_time;
	time_t	start_time;

	if (philo == NULL)
		return ;
	curr_time = pl_get_time(); // might have to lock this
	start_time = philo->rules->start_time; // this will always be constant
	usleep(1000);
	printf(WHT"%6ld %3d ", curr_time - start_time, philo->id + 1);
	if (state == FORK)
		printf("%-4s", "has ");
	else if (state != DIED)
		printf("%-4s", "is ");
	if (state == FORK)
		printf(PUR"taken a fork\n"DEF);
	else if (state == EAT)
		printf(GRN"eating\n"DEF);
	else if (state == SLEEP)
		printf(CYN"sleeping\n"DEF);
	else if (state == THINK)
		printf(YLW"thinking\n"DEF);
	else if (state == DIED)
		printf(RED"%8s\n"DEF, "died");
}
