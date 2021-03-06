/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barbare <barbare@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/09 14:36:09 by barbare           #+#    #+#             */
/*   Updated: 2017/03/24 14:56:02 by mbarbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "malloc.h"
#include <stdalign.h>

t_unit		*get_unit(void)
{
	static t_unit unit = { NULL, NULL, NULL, 0, PTHREAD_MUTEX_INITIALIZER};

	if (unit.pagesize == 0)
		unit.pagesize = (size_t)getpagesize();
	return (&unit);
}

void		*calloc(size_t count, size_t size)
{
	void		*mem;

	mem = malloc(size * count);
	pthread_mutex_lock(&(get_unit()->mutex));
	ft_bzero(mem, size * count);
	pthread_mutex_unlock(&(get_unit()->mutex));
	return (mem);
}

void		*malloc(size_t size)
{
	size_t		align;
	void		*mem;

	pthread_mutex_lock(&(get_unit()->mutex));
	align = ALIGN4(size);
	if (align <= (size_t)(SMALL))
		mem = insert_malloc(&get_unit()->tiny, align, tiny);
	else if (align <= (size_t)(LARGE))
		mem = insert_malloc(&get_unit()->small, align, small);
	else
		mem = insert_malloc(&get_unit()->big, align, big);
	pthread_mutex_unlock(&(get_unit()->mutex));
	return (mem);
}
