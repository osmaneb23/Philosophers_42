/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obouayed <obouayed@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 17:12:14 by obouayed          #+#    #+#             */
/*   Updated: 2024/10/14 18:17:58 by obouayed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <limits.h>   // INT_MAX and INT_MIN values
# include <pthread.h>  // Threads and mutexes functions
# include <stdbool.h>  // Boolean type and values (true, false)
# include <stdint.h>   // Integer types (uint64_t)
# include <stdio.h>    // Standard input/output functions (printf)
# include <stdlib.h>   // Memory allocation functions (malloc, free)
# include <string.h>   // String functions (memset)
# include <sys/time.h> // Time functions (gettimeofday)
# include <unistd.h>   // Standard symbolic constants and types (usleep)

/*
Structure to store the data of a philosopher:
- thread: thread of the philosopher
- id: id of the philosopher (from 1 to nbr_of_philos)
- nbr_of_meals: number of meals the philosopher has eaten
- is_dead: boolean to know if the philosopher is dead
- all_meals_eaten: boolean to know if the philosopher has eaten all his meals
- last_meal: time of the last meal in milliseconds
- left_fork: pointer to the left fork of the philosopher
- right_fork: pointer to the right fork of the philosopher
- info: pointer to the simulation data
*/
typedef struct s_philosopher
{
	pthread_t					thread;
	unsigned int				id;
	unsigned int				nbr_of_meals;
	bool						is_dead;
	bool						all_meals_eaten;
	uint64_t					last_meal;
	pthread_mutex_t				*left_fork;
	pthread_mutex_t				*right_fork;
	struct s_simulation_data	*info;
}								t_philosopher;

/*
Structure to store the data of the simulation:
- philosophers: array of philosophers
- monitor: thread to monitor the philosophers
- nbr_of_philos: number of philosophers
- time_to_die: time in milliseconds before a philosopher dies
- time_to_eat: time in milliseconds for a philosopher to eat
- time_to_sleep: time in milliseconds for a philosopher to sleep
- nbr_of_meals: number of meals a philosopher must eat before
the simulation ends (optional)
- start_time: start time of the simulation in milliseconds
- forks: array of mutexes to represent the forks
- stop: boolean to know if the simulation must stop
- print_mutex: mutex to protect the print function
- death_mutex: mutex to protect the death of a philosopher
- stop_mutex: mutex to protect the stop boolean
- meal_mutex: mutex to protect the number of meals eaten
by a philosopher
*/
typedef struct s_simulation_data
{
	t_philosopher				*philosophers;
	pthread_t					monitor;
	unsigned int				nbr_of_philos;
	uint64_t					time_to_die;
	uint64_t					time_to_eat;
	uint64_t					time_to_sleep;
	int							nbr_of_meals;
	uint64_t					start_time;
	pthread_mutex_t				*forks;
	bool						stop;
	pthread_mutex_t				print_mutex;
	pthread_mutex_t				death_mutex;
	pthread_mutex_t				stop_mutex;
	pthread_mutex_t				meal_mutex;
}								t_simulation_data;

// Verification functions

int								verif_parsing(int ac, char **av);

// Init functions

int								init_simulation(t_simulation_data *simulation,
									int ac, char **av);
int								init_mutexes(t_simulation_data *simulation);
int								init_threads(t_simulation_data *simulation);
int								join_threads(t_simulation_data *simulation);

// Routine functions

void							*main_routine(void *arg);
void							*monitor_routine(void *arg);
void							eating(t_philosopher *philosopher);
void							sleeping(t_philosopher *philosopher);
void							thinking(t_philosopher *philosopher);

// Routine utils functions

void							check_meals(t_philosopher *philosopher);
int								check_death(t_simulation_data *simulation,
									unsigned int index);
int								check_all_death(t_simulation_data *simulation);
void							check_eat_count(t_simulation_data *simulation);
void							release_forks(t_philosopher *philosopher);
void							take_forks(t_philosopher *philosopher);
void							handle_forks(t_philosopher *philosopher,
									pthread_mutex_t *first_fork,
									pthread_mutex_t *second_fork);

// Utils functions

void							ft_putstr_fd(char *s, int fd);
long long						ft_atopll(const char *nptr);
int								ft_atopi(const char *nptr);
uint64_t						get_current_time(void);
int64_t							ft_mssleep(t_simulation_data *simulation,
									uint64_t time);
void							safe_printf(char *str, unsigned int index,
									uint64_t time,
									t_simulation_data *simulation);

// Cleaning functions

int								cleanup(t_simulation_data *simulation,
									bool error, char *message);
int								clean_mutexes(t_simulation_data *simulation);

#endif