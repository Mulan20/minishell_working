/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkham <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 12:30:03 by nkham             #+#    #+#             */
/*   Updated: 2026/03/23 12:30:05 by nkham            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int last_exit_status = 0;

int get_last_exit_status(void)
{
    return (last_exit_status);
}

void set_last_exit_status(int status)
{
    last_exit_status = status;
}
