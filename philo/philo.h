/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wricky-t <wricky-t@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/10 15:15:04 by wricky-t          #+#    #+#             */
/*   Updated: 2023/02/17 16:08:12 by wricky-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

/* ====== LIBRARIES ====== */
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>

/* ====== TEXT STYLING ====== */
# define RED "\033[1;31m"
# define GRN "\033[1;32m"
# define YLW "\033[1;33m"
# define BLU "\033[1;34m"
# define PUR "\033[1;35m"
# define CYN "\033[1;36m"
# define WHT "\033[1;37m"
# define DEF "\033[0m"

/* ====== ENUMS ====== */

/**
 * @brief Enum for philo & simulation states
*/
typedef enum e_state
{
	FORK,
	EAT,
	SLEEP,
	THINK,
	DIED,
	NOTFULL,
	FULL,
	RUN,
	END
}		t_state;

/**
 * @brief Enum for mutex's actions
*/
typedef enum e_fork_action
{
	TAKE,
	RETURN
}		t_fork_action;

/**
 * @brief Enum for errors
*/
typedef enum e_error
{
	INVALID_ARGS_TOTAL,
	NON_NUMERIC_ARGS,
	NEGATIVE_ARGS,
	CREATE_THD_FAILED,
	CREATE_MUT_FAILED,
	DESTROY_MUT_FAILED,
	ERROR_TOTAL
}		t_error;

/* ====== STRUCTS ====== */

/**
 * @brief Struct for the simulation rules
 * 
 * @param sim_state 	The state of the simulation
 * @param start_time	The start time of the simulation
 * @param philo_total 	Number of philos
 * @param time_to_die 	Time to die in ms
 * @param time_to_eat 	Time to eat in ms
 * @param time_to_sleep Time to sleep in ms
 * @param iteration 	Number of simulation
*/
typedef struct s_rules
{
	t_state	sim_state;
	time_t	start_time;
	int		philo_total;
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
	int		iteration;
	int		philo_full;
}		t_rules;

/**
 * @brief Struct for each philo
 * 
 * @param id 				Philo ID
 * @param meal_count 		Number of meal eaten
 * @param full 				Philo's stomach lmao
 * @param last_ate 			Philo's last ate time
 * @param me 				The thread
 * @param left_fork 		Left fork (mutex), own index's fork
 * @param right_fork		Right fork (mutex) own index - 1's fork
 * @param last_ate_lock 	The mutex for last_ate
 * @param meal_count_lock 	The mutex for meal_count
 * @param rules 			The simulation rules
 * 
 * @attention These info should be private for each philo only
*/
typedef struct s_philo
{
	int				id;
	int				meal_count;
	int				full;
	time_t			last_ate;
	pthread_t		me;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	last_ate_lock;
	pthread_mutex_t	meal_count_lock;
	t_rules			*rules;
}		t_philo;

/**
 * @brief Struct for simulation
 * 
 * @param philo_full 	The number of philo that are full
 * @param forks 		The fork mutexes
 * @param rules 		The rules of the simulation
 * 
 * @attention The `n` here refers to the number of iteration
*/
typedef struct s_simulation
{
	pthread_mutex_t	*forks;
	t_philo			*philos;
	t_rules			*rules;
}		t_simulation;

/* ====== FUNCTION PROTOTYPES ====== */

// Parse
int		pl_parse(int ac, char **av, t_rules *rules);

// Philos
void	pl_begin_simulation(t_rules *rules);

// Philos action
void	*pl_routine(void *arg);

// Monitor
void	*pl_monitor(void *arg);

// Message
int		pl_show_error(t_error error, int id);
void	pl_declare_state(t_philo *philo, t_state state);

// Utils
int		ft_atoi(const char *str);
int		ft_isdigit_str(char *str);
time_t	pl_get_time(void);
void	pl_usleep(time_t sec);

#endif