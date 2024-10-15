/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obouayed <obouayed@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 08:38:55 by obouayed          #+#    #+#             */
/*   Updated: 2024/10/15 14:01:08 by obouayed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

/*
Function to verify the parsing of the arguments:
- Check if the arguments are positive integers
(except for the number of meals that can be 0 if specified)
- If an error is found, the function returns 1
- If the number of meals is 0,the function returns 2
(it's useless to run the simulation)
- Otherwise, the function returns 0 (no error found)
*/
int	verif_parsing(int ac, char **av)
{
	int			nbr_of_philos;
	long long	time_to_die;
	long long	time_to_eat;
	long long	time_to_sleep;
	int			nbr_of_meals;

	nbr_of_philos = ft_atopi(av[1]);
	time_to_die = ft_atopll(av[2]);
	time_to_eat = ft_atopll(av[3]);
	time_to_sleep = ft_atopll(av[4]);
	nbr_of_meals = 0;
	if (ac == 6)
		nbr_of_meals = ft_atopi(av[5]);
	if (nbr_of_philos < 1 || time_to_die < 1 || time_to_eat < 1
		|| time_to_sleep < 1 || nbr_of_meals < 0)
		return (cleanup(NULL, 1, "Error: invalid arguments\n"));
	if (nbr_of_meals == 0 && ac == 6)
		return (2);
	return (0);
}

/*
Main function:
- Check the number of arguments
- Verify the parsing of the arguments
- Initialize the simulation
- If an error is found, the function returns 1
*/
int	main(int ac, char **av)
{
	t_simulation_data	simulation;
	unsigned int		i;

	simulation.stop = false;
	if (ac < 5 || ac > 6)
		return (cleanup(NULL, 1, "Error: invalid number of arguments\n"));
	i = verif_parsing(ac, av);
	if (i == 1)
		return (1);
	else if (i == 2)
		return (0);
	if (init_simulation(&simulation, ac, av))
		return (1);
	return (cleanup(&simulation, 0, NULL));
}
