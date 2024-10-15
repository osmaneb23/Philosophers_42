/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obouayed <obouayed@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 20:08:00 by obouayed          #+#    #+#             */
/*   Updated: 2024/10/15 14:08:09 by obouayed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

/*
Function to print a string to the standard output thread-safely:
- Lock the print mutex
- Print the string with the time and the philosopher index
- Unlock the print mutex
*/
void	safe_printf(char *str, unsigned int index, uint64_t time,
		t_simulation_data *simulation)
{
	bool	stop;

	pthread_mutex_lock(&simulation->stop_mutex);
	stop = simulation->stop;
	pthread_mutex_unlock(&simulation->stop_mutex);
	if (!stop)
	{
		pthread_mutex_lock(&simulation->print_mutex);
		printf("%lu | Philosopher %d %s", time, index, str);
		pthread_mutex_unlock(&simulation->print_mutex);
	}
}
