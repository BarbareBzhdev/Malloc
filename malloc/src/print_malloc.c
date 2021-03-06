/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tiny_malloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barbare <barbare@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/09 16:51:06 by barbare           #+#    #+#             */
/*   Updated: 2017/03/24 14:12:59 by mbarbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "malloc.h"

void			print_block(t_block *blk, const char *type)
{
	ft_putstr(type);
	ft_putstr(" memory : 0x");
	print_ptr((uintmax_t)blk->mem);
	ft_putstr(" - 0x");
	print_ptr((uintmax_t)(blk->mem + blk->size));
	ft_putendl("");
}

void			print_ptr(uintmax_t hex)
{
	int			cmp;
	uintmax_t	x;
	int			len;
	char		addr[20];

	cmp = 0;
	x = hex;
	len = ft_strlen(BASE16);
	if (hex == 0)
		ft_putstr("0");
	while (x > 0)
	{
		x /= len;
		cmp++;
	}
	addr[cmp] = '\0';
	while (--cmp >= 0)
	{
		addr[cmp] = BASE16[(hex % len)];
		hex /= len;
	}
	write(1, &addr[ft_strlen(addr) - sizeof(int64_t)], sizeof(int64_t));
}

static void		rec_block_read(t_block *block, size_t *total)
{
	if (block == NULL)
		return ;
	if (block->free != FREE)
	{
		ft_putendl("");
		ft_putstr("0x");
		print_ptr((uintmax_t)block->mem);
		ft_putstr(" - 0x");
		print_ptr((uintmax_t)(block->mem + block->size));
		ft_putstr(" : ");
		ft_putnbr(block->size);
		ft_putstr(" octets.");
		*total += block->size;
	}
	return (rec_block_read(block->next, total));
}

static size_t	read_by_meta(t_metadata *meta, const char *type)
{
	size_t			total;

	total = 0;
	ft_putstr("\nSHOW ALLOC MEM ");
	ft_putstr(type);
	ft_putstr(" : 0x");
	print_ptr((uintmax_t)meta);
	while (meta)
	{
		rec_block_read((void *)meta + sizeof(t_metadata), &total);
		meta = meta->next;
	}
	return (total);
}

void			show_alloc_mem(void)
{
	size_t			total;

	pthread_mutex_lock(&(get_unit()->mutex));
	total = read_by_meta(get_unit()->tiny, "TINY");
	total += read_by_meta(get_unit()->small, "SMALL");
	total += read_by_meta(get_unit()->big, "BIG");
	ft_putstr("\n\nTotal : ");
	ft_putnbr(total);
	ft_putendl(" octets.");
	pthread_mutex_unlock(&(get_unit()->mutex));
}
