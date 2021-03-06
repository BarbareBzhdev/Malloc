/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barbare <barbare@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/14 12:57:39 by barbare           #+#    #+#             */
/*   Updated: 2017/03/24 13:12:05 by mbarbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "malloc.h"

static int	check_block(t_block *block, void *mem)
{
	if (block->mem == mem)
	{
		if (getenv("MALLOC_DEBUG") != NULL)
			print_block(block, "Free");
		block->free = FREE;
		return (TRUE);
	}
	else if (block->next)
		return (check_block(block->next, mem));
	else
		return (FALSE);
}

static int	check_free(t_metadata *md, void *mem)
{
	if (check_block((void *)md + sizeof(t_metadata), mem) == TRUE)
		return (TRUE);
	else if (md->next != NULL)
		return (check_free(md->next, mem));
	else
		return (FALSE);
}

static int	check_block_with_munmap(t_block *block, void *mem)
{
	if (block->mem == mem)
	{
		munmap(block->mem, block->size);
		block->free = FREE;
		return (TRUE);
	}
	else if (block->next)
		return (check_block_with_munmap(block->next, mem));
	else
		return (FALSE);
}

static int	check_free_with_munmap(t_metadata *md, void *mem)
{
	if (check_block_with_munmap((void *)md + sizeof(t_metadata), mem) == TRUE)
		return (TRUE);
	else if (md->next != NULL)
		return (check_free_with_munmap(md->next, mem));
	else
		return (FALSE);
}

void		free(void *mem)
{
	pthread_mutex_lock(&(get_unit()->mutex));
	if (mem == NULL ||
			(get_unit()->tiny && check_free(get_unit()->tiny, mem)) ||
				(get_unit()->small && check_free(get_unit()->small, mem)) ||
					(get_unit()->big &&
						check_free_with_munmap(get_unit()->big, mem)))
	{
		pthread_mutex_unlock(&(get_unit()->mutex));
		return ;
	}
	pthread_mutex_unlock(&(get_unit()->mutex));
}
