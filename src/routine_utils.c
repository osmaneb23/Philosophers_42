/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obouayed <obouayed@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 18:12:00 by obouayed          #+#    #+#             */
/*   Updated: 2024/10/14 17:56:20 by obouayed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

/*
Function to check if one of the philosophers is dead or not:
- Loop through all the philosophers
- Calculate the time since the last meal of the philosopher
- If the time since the last meal is greater than the time to die,
the philosopher died
- If a philosopher is dead, the function returns 1
- If a philosopher is not dead, the function returns 0
*/
int	check_all_death(t_simulation_data *simulation)
{
	unsigned int	i;

	i = 0;
	while (i < simulation->nbr_of_philos)
	{
		pthread_mutex_lock(&simulation->death_mutex);
		if (check_death(simulation, i))
		{
			pthread_mutex_lock(&simulation->stop_mutex);
			simulation->stop = 1;
			pthread_mutex_unlock(&simulation->stop_mutex);
			pthread_mutex_unlock(&simulation->death_mutex);
			return (1);
		}
		pthread_mutex_unlock(&simulation->death_mutex);
		i++;
	}
	return (0);
}

/*
Function to check if a philosopher is dead:
- Lock the meal mutex
- Calculate the time since the last meal of the philosopher
- If the time since the last meal minus the actual time is less than
the time to die, the philosopher is dead
- If the philosopher is dead, the function returns 1
- Otherwise, the function returns 0
*/
int	check_death(t_simulation_data *simulation, unsigned int index)
{
	uint64_t	death_formula;

	pthread_mutex_lock(&simulation->meal_mutex);
	death_formula = simulation->philosophers[index].last_meal
		- get_current_time() + simulation->time_to_die;
	pthread_mutex_unlock(&simulation->meal_mutex);
	if (simulation->philosophers[index].is_dead)
		return (1);
	pthread_mutex_lock(&simulation->meal_mutex);
	if (death_formula <= 0 && !simulation->philosophers[index].all_meals_eaten)
	{
		safe_printf("died\n", index + 1, get_current_time()
			- simulation->start_time, simulation);
		simulation->philosophers[index].is_dead = 1;
		pthread_mutex_unlock(&simulation->meal_mutex);
		return (1);
	}
	pthread_mutex_unlock(&simulation->meal_mutex);
	return (0);
}

/*
Function to check if a philosopher has eaten all his meals:
- Lock the death mutex and the meal mutex
- If the philosopher is dead or has eaten all his meals, the function returns
- If the number of meals is equal to the number of meals the philosopher
must eat, the philosopher has eaten all his meals and the function returns
*/
void	check_meals(t_philosopher *philosopher)
{
	pthread_mutex_lock(&philosopher->info->death_mutex);
	pthread_mutex_lock(&philosopher->info->meal_mutex);
	if (philosopher->is_dead || philosopher->all_meals_eaten)
	{
		pthread_mutex_unlock(&philosopher->info->death_mutex);
		pthread_mutex_unlock(&philosopher->info->meal_mutex);
		return ;
	}
	pthread_mutex_unlock(&philosopher->info->death_mutex);
	if (philosopher->nbr_of_meals == philosopher->info->nbr_of_meals)
	{
		safe_printf("has eaten all his meals\n", philosopher->id,
			get_current_time() - philosopher->info->start_time,
			philosopher->info);
		philosopher->all_meals_eaten = 1;
		pthread_mutex_unlock(&philosopher->info->meal_mutex);
		return ;
	}
	pthread_mutex_unlock(&philosopher->info->meal_mutex);
}

/*
Function to check if all the philosophers have eaten all their meals:
- Loop through all the philosophers
- Lock the meal mutex
- If the philosopher has not eaten all his meals, the function returns
- If all the philosophers have eaten all their meals, the function sets the
stop boolean to 1
*/
void	check_eat_count(t_simulation_data *simulation)
{
	unsigned int	i;

	i = 0;
	while (i < simulation->nbr_of_philos && simulation->nbr_of_meals != -1)
	{
		pthread_mutex_lock(&simulation->meal_mutex);
		if (simulation->philosophers[i].all_meals_eaten)
			i++;
		else
		{
			pthread_mutex_unlock(&simulation->meal_mutex);
			return ;
		}
		pthread_mutex_unlock(&simulation->meal_mutex);
	}
	pthread_mutex_lock(&simulation->stop_mutex);
	if (i == simulation->nbr_of_philos)
		simulation->stop = 1;
	pthread_mutex_unlock(&simulation->stop_mutex);
}
