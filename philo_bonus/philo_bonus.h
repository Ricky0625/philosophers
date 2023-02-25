/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wricky-t <wricky-t@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 11:06:53 by wricky-t          #+#    #+#             */
/*   Updated: 2023/02/25 16:35:25 by wricky-t         ###   ########.fr       */
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
# include <signal.h>
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

/* ====== MACROS ====== */
# define FORK_SEM "/fork_sem"
# define DECLARE_SEM "/declare_sem"
# define SIM_SEM "/sim_sem"
# define FULL_SEM "/full_sem"
# define LAST_ATE_SEM "/lastate_sem"

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
 * @param LAST_ATE To setup semaphore for last_ate
 * @param MEAL     To setup semaphore for meal_count
*/
typedef enum e_lock_type
{
	LAST_ATE,
	MEAL
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
	OPEN_SEM_FAILED,
	FORK_FAILED,
	CREATE_THD_FAILED,
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
	sem_t	*declare_sem;
	sem_t	*sim_sem;
	sem_t	*full_sem;
}		t_locks;

/**
 * @brief Struct for the simulation rules
 * 
 * @param start_time	The start time of the simulation
 * @param philo_total 	Number of philos
 * @param time_to_die 	Time to die in ms
 * @param time_to_eat 	Time to eat in ms
 * @param time_to_sleep Time to sleep in ms
 * @param iteration 	Number of simulation
*/
typedef struct s_rules
{
	time_t	start_time;
	int		philo_total;
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
	int		iteration;
	t_locks	locks;
	sem_t	*forks;
	pid_t	*pids;
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
	pid_t	me;
	int		id;
	int		meal_count;
	int		full;
	time_t	last_ate;
	sem_t	*last_ate_sem;
	t_rules	*rules;
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
	t_philo	*philos;
	t_rules	*rules;
}		t_simulation;

/* ====== FUNCTION PROTOTYPES ====== */

// Parse
int		pl_parse(int ac, char **av, t_rules *rules);

// Philos
void	pl_begin_simulation(t_rules *rules);

// Philos action
void	pl_routine(t_philo *philo);

// Monitor
void	*pl_monitor(void *philo);
void	pl_check_full(t_rules *rules);
int		pl_monitor_sim(t_rules *rules);

// Semaphore
int		pl_setup_shared_sem(t_locks *locks);
int		pl_setup_philo_sem(t_philo *philo);

// Message
int		pl_show_error(t_error error, int id);
void	pl_declare_state(t_philo *philo, t_state state);

// Utils
time_t	pl_get_time(void);
void	pl_usleep(time_t sec);
int		pl_sem_open(sem_t **sem, char *name, int value);
char	*ft_itoa(int n);
char	*ft_strjoin(char const *s1, char const *s2);

#endif