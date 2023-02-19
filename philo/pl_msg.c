/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pl_msg.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wricky-t <wricky-t@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 10:13:17 by wricky-t          #+#    #+#             */
/*   Updated: 2023/02/19 15:41:13 by wricky-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Show error message
 * @param error Error type
 * @param id The id of the philo
 * 
 * @details
 * 1. INVALID_ARGS_TOTAL - The number of args is nor 4 or 5.
 * 2. NON_NUMERIC_ARGS - Found non numeric arguments.
 * 3. NEGATIVE_ARGS - Found negative arguments.
 * 4. CREATE_THD_FAILED - Failed to create thread.
 * 5. CREATE_MUT_FAILED - Failed to create mutex.
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
	else if (error == CREATE_THD_FAILED)
		printf("Failed to create PHILO %d\n", id);
	else if (error == CREATE_MUT_FAILED)
		printf("Failed to create MUTEX %d\n", id);
	else if (error == DESTROY_MUT_FAILED)
		printf("Failed to destroy MUTEX %d\n", id);
	if (!(error >= CREATE_THD_FAILED && error < ERROR_TOTAL))
		printf(GRN"Run `make help` for help.\n"DEF);
	return (0);
}

/**
 * @brief Declare the state of a philo
 * @param philo The target philo
 * @param state The state of the philo
*/
/**
 * TODO: Might have to lock and unlock "curr_time"
*/
void	pl_declare_state(t_philo *philo, t_state state)
{
	time_t	curr_time;
	time_t	start_time;

	if (philo == NULL || pl_get_sim_state(philo) == END)
		return ;
	pthread_mutex_lock(&philo->rules->locks.declare_lock);
	curr_time = pl_get_time();
	start_time = philo->rules->start_time;
	// printf("%ld %d ", curr_time - start_time, philo->id + 1);
	printf("%8ld %3d ", curr_time - start_time, philo->id + 1);
	if (state == FORK)
		printf(PUR"has taken a fork\n"DEF);
	else if (state == EAT)
		printf(GRN"is eating\n"DEF);
	else if (state == SLEEP)
		printf(CYN"is sleeping\n"DEF);
	else if (state == THINK)
		printf(YLW"is thinking\n"DEF);
	else if (state == DIED)
		printf(RED"%s\n"DEF, "died");
	pthread_mutex_unlock(&philo->rules->locks.declare_lock);
}
