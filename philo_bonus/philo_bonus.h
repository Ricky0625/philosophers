/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wricky-t <wricky-t@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 10:42:30 by wricky-t          #+#    #+#             */
/*   Updated: 2023/02/22 12:21:23 by wricky-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

/* ====== LIBRARIES ====== */
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <pthread.h>
# include <semaphore.h>

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
 * 
 * @param FORK		Taken a fork
 * @param EAT		Eating
 * @param SLEEP		Sleeping
 * @param THINK		Thinking
 * @param DIED		Died
 * @param NOTFULL	Not full (optional mode)
 * @param FULL		Full (optional mode)
 * @param RUN		Simulation still on-going
 * @param END		Simulation ended
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
	HALT,
	RUN,
	END
}		t_state;

/**
 * @brief Enum for fork mutex's actions
 * 
 * @param TAKE		Take fork (lock fork mutex)
 * @param RETURN	Return fork (unlock fork mutex)
*/
typedef enum e_fork_action
{
	TAKE,
	RETURN
}		t_fork_action;

/**
 * @brief Enum for lock type
 * 
 * @param SHARED 	The mutex is shared among all philo
 * @param PHILO		The mutex is only for the assigned philo
*/
typedef enum e_lock_type
{
	SHARED,
	PHILO
}		t_lock_type;

/**
 * @brief Enum for errors
 * 
 * @param INVALID_ARGS_TOTAL	Invalid arguments count
 * @param NON_NUMERIC_ARGS		Consists of non numeric arguments
 * @param NEGATIVE_ARGS			Consists of non positive numeric arguments
 * @param INVALID_OPTION		Optional option not more than 0
 * @param CREATE_THD_FAILED		Failed to create thread
 * @param CREATE_MUT_FAILED		Failed to create mutex
 * @param DESTROY_MUT_FAILED	Failed to destroy mutex
*/
typedef enum e_error
{
	INVALID_ARGS_TOTAL,
	NON_NUMERIC_ARGS,
	NEGATIVE_ARGS,
	INVALID_OPTION,
	CREATE_THD_FAILED,
	CREATE_MUT_FAILED,
	DESTROY_MUT_FAILED,
	ERROR_TOTAL
}		t_error;

/* ====== STRUCTS ====== */

/**
 * @brief A struct that store all the additional mutexes (shared)
 * 
 * @param declare_lock		Philo's declare mutex
 * @param sim_state_lock	Philo's simulation state mutex
*/
typedef struct s_locks
{
	pthread_mutex_t	declare_lock;
	pthread_mutex_t	sim_state_lock;
	pthread_mutex_t	death_lock;
	pthread_mutex_t	full_lock;
}		t_locks;

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
	t_locks	locks;
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
 * @param locks				The additional locks that prevent data race
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
int		pl_lock_setup(t_locks *locks, t_philo *philo, t_lock_type type);

// Philos action
void	*pl_routine(void *arg);
void	pl_fork_action(t_philo *philo, t_fork_action act);

// Monitor
void	*pl_monitor(void *arg);

// Monitor utils
time_t	pl_get_last_ate(t_philo *philo);
t_state	pl_get_sim_state(t_philo *philo);
int		pl_get_meal_count(t_philo *philo);

// Message
int		pl_show_error(t_error error, int id);
void	pl_declare_state(t_philo *philo, t_state state);

// Utils
int		ft_atoi(const char *str);
int		ft_isdigit_str(char *str);
time_t	pl_get_time(void);
void	pl_usleep(time_t sec);

#endif