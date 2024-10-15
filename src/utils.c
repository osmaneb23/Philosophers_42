/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obouayed <obouayed@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 17:10:23 by obouayed          #+#    #+#             */
/*   Updated: 2024/10/14 17:31:15 by obouayed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

/*
Function to make a thread sleep for a certain time in milliseconds:
- If the time is greater than the time to die,
the time is set to the time to die
- If usleep fails, the function returns -1
- Otherwise, the function returns 0
*/
int64_t	ft_mssleep(t_simulation_data *simulation, uint64_t time)
{
	if (time >= simulation->time_to_die)
		time = simulation->time_to_die;
	if (usleep(time * 1000))
	{
		ft_putstr_fd("Error: usleep failed\n", 2);
		simulation->stop = 1;
		return (-1);
	}
	return (0);
}

/*
Function to get the current time in milliseconds:
- Convert seconds to milliseconds and add the microseconds
- Return the current time in milliseconds
- Return UINT64_MAX if an error is found
*/
uint64_t	get_current_time(void)
{
	struct timeval	tv;
	uint64_t		milliseconds;

	if (gettimeofday(&tv, NULL) == -1)
		return (UINT64_MAX);
	milliseconds = (tv.tv_sec * 1000LL) + (tv.tv_usec / 1000LL);
	return (milliseconds);
}

// Function to print a string to a file descriptor
void	ft_putstr_fd(char *s, int fd)
{
	int	i;

	i = 0;
	if (s == NULL)
		return ;
	while (s[i])
	{
		write(fd, &s[i], 1);
		i++;
	}
}

/*
Function to convert a string to a positive integer:
- Return -1 if an error is found (invalid characters, overflow, etc.)
*/
int	ft_atopi(const char *nptr)
{
	unsigned int	i;
	int				result;

	i = 0;
	result = 0;
	if (nptr[i] < '0' || nptr[i] > '9')
		return (-1);
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		result = result * 10 + (nptr[i] - '0');
		i++;
	}
	if ((nptr[i] < '0' || nptr[i] > '9') && nptr[i] != '\0' && nptr[i] != ' ')
		return (-1);
	if (result > INT_MAX)
		return (-1);
	return (result);
}

/*
Function to convert a string to a positive long long:
- Return -1 if an error is found (invalid characters, overflow, etc.)
*/
long long	ft_atopll(const char *nptr)
{
	unsigned int	i;
	long long		result;

	i = 0;
	result = 0;
	if (nptr[i] < '0' || nptr[i] > '9')
		return (-1);
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		result = result * 10 + (nptr[i] - '0');
		i++;
	}
	if ((nptr[i] < '0' || nptr[i] > '9') && nptr[i] != '\0' && nptr[i] != ' ')
		return (-1);
	if (result > LLONG_MAX)
		return (-1);
	return (result);
}
