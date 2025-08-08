#include "minishell.h"

void    *ft_malloc(int size, t_free **free_list)
{
    void *p;

    p = malloc(size);
    if(!p)
    {
        perror("malloc");
        ft_free(free_list, 1);
        exit(EXIT_FAILURE);
    }
    return (p);
}

void    ft_free(t_free **list, int flag)
{
    t_free *temp;
    t_free *new;
    t_free *prev;

    new = malloc(sizeof(t_free));
    if (!new)
    {
        perror("malloc");
        ft_free(list, 1);
        exit(EXIT_FAILURE);
    }
    if (flag == 1)
    {
        temp = *list;
        while (temp)
        {
            prev = temp->next;
            free(temp->ptr);
            free(temp);
            temp = prev;
        }
    }
    else if (flag == 2)
    {
        temp = *list;
        while(temp->next)
            temp = temp->next;
        temp->next = new;
        new->next = NULL;
    }
}
