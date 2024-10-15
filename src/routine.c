/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obouayed <obouayed@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 18:08:11 by obouayed          #+#    #+#             */
/*   Updated: 2024/10/14 18:26:08 by obouayed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

/*
Function to run the monitor routine:
- Loop until the simulation must stop
- If the number of philosophers is 1, the function returns
- Check if a philosopher is dead
- If a philosopher is dead, the monitor stops
- Check if all the philosophers have eaten all their meals
- If all the philosophers have eaten all their meals, the monitor stops
*/
void	*monitor_routine(void *arg)
{
	t_simulation_data	*simulation;

	simulation = (t_simulation_data *)arg;
	while (!simulation->stop)
	{
		if (simulation->nbr_of_philos == 1 || check_all_death(simulation))
			break ;
		check_eat_count(simulation);
		usleep(10);
	}
	return (NULL);
}

/*
Function to run the main routine of the philosophers:
- Check if the philosopher has eaten all his meals
- If the philosopher has eaten all his meals, the function returns
- The philosopher takes the forks
- If the number of philosophers is 1, the function returns after his death
(inside take_forks)
- The philosopher eats
- The philosopher releases the forks
- The philosopher sleeps
- The philosopher thinks
- If the simulation must stop, the function returns
*/
void	*main_routine(void *arg)
{
	t_philosopher	*philosopher;

	philosopher = (t_philosopher *)arg;
	while (1)
	{
		check_meals(philosopher);
		if (philosopher->all_meals_eaten)
			return (NULL);
		take_forks(philosopher);
		if (philosopher->info->nbr_of_philos == 1)
			return (NULL);
		eating(philosopher);
		release_forks(philosopher);
		sleeping(philosopher);
		thinking(philosopher);
		pthread_mutex_lock(&philosopher->info->stop_mutex);
		if (philosopher->info->stop)
		{
			pthread_mutex_unlock(&philosopher->info->stop_mutex);
			return (NULL);
		}
		pthread_mutex_unlock(&philosopher->info->stop_mutex);
	}
	return (NULL);
}

/*
Function to make a philosopher eat:
- Print the time and the philosopher that is eating
- Lock the meal mutex
- Update the last meal of the philosopher
- Unlock the meal mutex
- Make the philosopher sleep for the time to eat, but if the time to eat is
greater than the time to die, the philosopher sleeps for the time to die
and dies
- Lock the meal mutex
- If the number of meals is not -1, increment the number of meals eaten
- Unlock the meal mutex
*/
void	eating(t_philosopher *philosopher)
{
	safe_printf("is eating\n", philosopher->id, get_current_time()
		- philosopher->info->start_time, philosopher->info);
	pthread_mutex_lock(&philosopher->info->meal_mutex);
	philosopher->last_meal = get_current_time();
	pthread_mutex_unlock(&philosopher->info->meal_mutex);
	ft_mssleep(philosopher->info, philosopher->info->time_to_eat);
	pthread_mutex_lock(&philosopher->info->meal_mutex);
	if (philosopher->info->nbr_of_meals != -1)
		philosopher->nbr_of_meals++;
	pthread_mutex_unlock(&philosopher->info->meal_mutex);
}

/*
Function printing the time and the philosopher that is sleeping:
- The philosopher sleeps for the time to sleep
but if the time to sleep is greater than the time to die,
the philosopher sleeps for the time to die and dies
*/
void	sleeping(t_philosopher *philosopher)
{
	safe_printf("is sleeping\n", philosopher->id, get_current_time()
		- philosopher->info->start_time, philosopher->info);
	ft_mssleep(philosopher->info, philosopher->info->time_to_sleep);
}

// Function printing the time and the philosopher that is thinking
void	thinking(t_philosopher *philosopher)
{
	safe_printf("is thinking\n", philosopher->id, get_current_time()
		- philosopher->info->start_time, philosopher->info);
}
