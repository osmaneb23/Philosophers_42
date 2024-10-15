/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obouayed <obouayed@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 16:37:28 by obouayed          #+#    #+#             */
/*   Updated: 2024/10/15 15:17:16 by obouayed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

/*
Function to take the forks of a philosopher:
- If the philosopher has an even id, he takes the right fork first
- If the philosopher has an odd id, he takes the left fork first
- Call the function to handle the forks
*/
void	take_forks(t_philosopher *philosopher)
{
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;

	if (philosopher->id % 2 == 0)
	{
		first_fork = philosopher->right_fork;
		second_fork = philosopher->left_fork;
	}
	else
	{
		first_fork = philosopher->left_fork;
		second_fork = philosopher->right_fork;
	}
	handle_forks(philosopher, first_fork, second_fork);
}

/*
Function to handle the forks of a philosopher:
- Lock the first fork
- If the philosopher is alone, he dies
- Lock the second fork
*/
void	handle_forks(t_philosopher *philosopher, pthread_mutex_t *first_fork,
		pthread_mutex_t *second_fork)
{
	pthread_mutex_lock(first_fork);
	safe_printf("has taken a fork\n", philosopher->id, get_current_time()
		- philosopher->info->start_time, philosopher->info);
	if (philosopher->info->nbr_of_philos == 1)
	{
		ft_mssleep(philosopher->info, philosopher->info->time_to_die);
		safe_printf("died\n", philosopher->id, get_current_time()
			- philosopher->info->start_time, philosopher->info);
		pthread_mutex_lock(&philosopher->info->death_mutex);
		philosopher->is_dead = 1;
		pthread_mutex_unlock(&philosopher->info->death_mutex);
		pthread_mutex_unlock(first_fork);
		return ;
	}
	pthread_mutex_lock(second_fork);
	safe_printf("has taken a fork\n", philosopher->id, get_current_time()
		- philosopher->info->start_time, philosopher->info);
}

/*
Function to release the forks of a philosopher:
- If the philosopher has an even id, he releases the left fork first
- If the philosopher has an odd id, he releases the right fork first
*/
void	release_forks(t_philosopher *philosopher)
{
	if (philosopher->id % 2 == 0)
	{
		pthread_mutex_unlock(philosopher->right_fork);
		usleep(200);
		pthread_mutex_unlock(philosopher->left_fork);
	}
	else
	{
		pthread_mutex_unlock(philosopher->left_fork);
		usleep(200);
		pthread_mutex_unlock(philosopher->right_fork);
	}
}
