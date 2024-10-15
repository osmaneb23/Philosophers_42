/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obouayed <obouayed@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 15:12:27 by obouayed          #+#    #+#             */
/*   Updated: 2024/10/15 14:48:42 by obouayed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

/*
Function to initialize the simulation:
- Initialize the simulation data
- Allocate memory for the philosophers
- Initialize the philosophers
- Initialize the mutexes
- Initialize the threads (inside the mutexes init)
- Join the threads
- If an error is found, the function returns 1
- Otherwise, the function returns 0
*/
int	init_simulation(t_simulation_data *simulation, int ac, char **av)
{
	t_philosopher	*philosophers;

	simulation->philosophers = NULL;
	simulation->nbr_of_philos = ft_atopi(av[1]);
	simulation->time_to_die = ft_atopll(av[2]);
	simulation->time_to_eat = ft_atopll(av[3]);
	simulation->time_to_sleep = ft_atopll(av[4]);
	simulation->start_time = get_current_time();
	if (simulation->start_time == UINT64_MAX)
		return (cleanup(simulation, 1, "Error: gettimeofday failed\n"));
	if (ac == 6)
		simulation->nbr_of_meals = ft_atopi(av[5]);
	else
		simulation->nbr_of_meals = -1;
	simulation->philosophers = malloc(sizeof(t_philosopher)
			* simulation->nbr_of_philos);
	if (!simulation->philosophers)
		return (cleanup(simulation, 1, "Error: malloc failed\n"));
	memset(simulation->philosophers, 0, sizeof(t_philosopher)
		* simulation->nbr_of_philos);
	if (init_mutexes(simulation))
		return (1);
	if (join_threads(simulation))
		return (1);
	return (0);
}

/*
Function to initialize the mutexes:
- Allocate memory for the forks
- Initialize the forks
- Initialize the print mutex
- Initialize the death mutex
- Initialize the stop mutex
- Initialize the meal mutex
- Initialize the threads
- If an error is found, the function returns 1
- Otherwise, the function returns 0
*/
int	init_mutexes(t_simulation_data *simulation)
{
	unsigned int	i;

	i = 0;
	simulation->forks = malloc(sizeof(pthread_mutex_t)
			* simulation->nbr_of_philos);
	if (!simulation->forks)
		return (cleanup(simulation, 1, "Error: malloc failed\n"));
	while (i < simulation->nbr_of_philos)
	{
		if (pthread_mutex_init(&simulation->forks[i], NULL))
			return (cleanup(simulation, 1,
					"Error: pthread_mutex_init failed\n"));
		i++;
	}
	if (pthread_mutex_init(&simulation->print_mutex, NULL))
		return (cleanup(simulation, 1, "Error: pthread_mutex_init failed\n"));
	if (pthread_mutex_init(&simulation->death_mutex, NULL))
		return (cleanup(simulation, 1, "Error: pthread_mutex_init failed\n"));
	if (pthread_mutex_init(&simulation->stop_mutex, NULL))
		return (cleanup(simulation, 1, "Error: pthread_mutex_init failed\n"));
	if (pthread_mutex_init(&simulation->meal_mutex, NULL))
		return (cleanup(simulation, 1, "Error: pthread_mutex_init failed\n"));
	if (init_threads(simulation))
		return (1);
	return (0);
}

/*
Function to initialize the threads:
- Create the monitor thread
- Create the threads of the philosophers
- Initialize the values of the philosophers
- Set the left and right forks of the philosophers
- If an error is found, the function returns 1
- Otherwise, the function returns 0
*/
int	init_threads(t_simulation_data *simulation)
{
	unsigned int	i;

	i = 0;
	while (i < simulation->nbr_of_philos)
	{
		simulation->philosophers[i].id = i + 1;
		simulation->philosophers[i].nbr_of_meals = 0;
		simulation->philosophers[i].is_dead = 0;
		simulation->philosophers[i].all_meals_eaten = 0;
		simulation->philosophers[i].last_meal = simulation->start_time;
		simulation->philosophers[i].left_fork = &simulation->forks[i];
		if (simulation->philosophers[i].id == simulation->nbr_of_philos)
			simulation->philosophers[i].right_fork = &simulation->forks[0];
		else
			simulation->philosophers[i].right_fork = &simulation->forks[i + 1];
		simulation->philosophers[i].info = simulation;
		if (pthread_create(&simulation->philosophers[i].thread, NULL,
				&main_routine, &simulation->philosophers[i]))
			return (cleanup(simulation, 1, "Error: pthread_create failed\n"));
		i++;
	}
	if (pthread_create(&simulation->monitor, NULL, &monitor_routine,
			simulation))
		return (cleanup(simulation, 1, "Error: pthread_create failed\n"));
	return (0);
}

/*
Function to join the threads of the philosophers and the monitor:
- Join the monitor thread
- Join the threads of the philosophers
- Wait for the threads to finish
- If an error is found, the function returns 1
- Otherwise, the function returns 0
*/
int	join_threads(t_simulation_data *simulation)
{
	unsigned int	i;

	i = 0;
	if (pthread_join(simulation->monitor, NULL))
		return (cleanup(simulation, 1, "Error: pthread_join failed\n"));
	while (i < simulation->nbr_of_philos)
	{
		if (pthread_join(simulation->philosophers[i].thread, NULL))
			return (cleanup(simulation, 1, "Error: pthread_join failed\n"));
		i++;
	}
	return (0);
}
