#include "minishell.h"

t_free *g_free_list = NULL;

void *ft_malloc(size_t size)
{
    void    *p;
    t_free  *node;

    p = malloc(size);
    if (!p)
    {
        perror("malloc");
        ft_free_all();
        exit(EXIT_FAILURE);
    }
    node = malloc(sizeof(*node));
    if (!node)
    {
        perror("malloc");
        free(p);
        ft_free_all();
        exit(EXIT_FAILURE);
    }
    node->ptr = p;
    node->next = g_free_list;
    g_free_list = node;
    return p;
}

void ft_free_one(void *ptr)
{
    t_free *temp = g_free_list;
    t_free *prev = NULL;

    if (!ptr)
        return;
    while (temp)
    {
        if (temp->ptr == ptr)
        {
            if (prev)
                prev->next = temp->next;
            else
                g_free_list = temp->next;
            if (temp->ptr)
                free(temp->ptr);
            free(temp);
            return;
        }
        prev = temp;
        temp = temp->next;
    }
}

void ft_free_all(void)
{
    t_free *temp = g_free_list;
    t_free *next;

    while (temp)
    {
        next = temp->next;
        if (temp->ptr)
            free(temp->ptr);
        free(temp);
        temp = next;
    }
    g_free_list = NULL;
}

char *ft_strdup_gc(const char *s)
{
    size_t len = 0;
    char *dup;
    size_t i;

    if (!s)
        return NULL;
    while (s[len])
        len++;
    dup = ft_malloc(len + 1);
    for (i = 0; i <= len; i++)
        dup[i] = s[i];
    return dup;
}
