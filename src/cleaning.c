/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaning.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obouayed <obouayed@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 16:15:48 by obouayed          #+#    #+#             */
/*   Updated: 2024/10/14 17:20:16 by obouayed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

/*
Function to destroy the mutexes:
- Loop through all the fork mutexes and destroy them
- Free the memory allocated for the forks
- Destroy the other mutexes
- If an error is found, the function prints the message to the standard error
*/
int	clean_mutexes(t_simulation_data *simulation)
{
	unsigned int	i;

	i = 0;
	if (simulation->forks)
	{
		while (i < simulation->nbr_of_philos)
		{
			if (pthread_mutex_destroy(&simulation->forks[i]))
				return (ft_putstr_fd("Error: pthread_mutex_destroy failed\n",
						2), 1);
			i++;
		}
		free(simulation->forks);
	}
	if (pthread_mutex_destroy(&simulation->print_mutex))
		return (ft_putstr_fd("Error: pthread_mutex_destroy failed\n", 2), 1);
	if (pthread_mutex_destroy(&simulation->death_mutex))
		return (ft_putstr_fd("Error: pthread_mutex_destroy failed\n", 2), 1);
	if (pthread_mutex_destroy(&simulation->stop_mutex))
		return (ft_putstr_fd("Error: pthread_mutex_destroy failed\n", 2), 1);
	if (pthread_mutex_destroy(&simulation->meal_mutex))
		return (ft_putstr_fd("Error: pthread_mutex_destroy failed\n", 2), 1);
	return (0);
}

/*
Function to free the memory allocated for the simulation:
- If philosophers is not NULL, free the memory allocated for it
- Clean the mutexes
- If an error is found, the function prints the message to the standard error
*/
int	cleanup(t_simulation_data *simulation, bool error, char *message)
{
	int	i;

	i = 0;
	if (simulation)
	{
		if (simulation->philosophers)
			free(simulation->philosophers);
		error = clean_mutexes(simulation);
	}
	if (error)
		return (ft_putstr_fd(message, 2), 1);
	return (0);
}
