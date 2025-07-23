#include "../minishell.h"

void	free_commands(t_cmd *cmds)
{
    t_cmd	*tmp;
    int		i;

    while (cmds)
    {
        tmp = cmds;
        cmds = cmds->next;
        i = 0;
        while (i < tmp->arg_count)
            free(tmp->args[i++]);
        free(tmp->args);
        if (tmp->input_file)
            free(tmp->input_file);
        if (tmp->output_file)
            free(tmp->output_file);
        if (tmp->heredoc_delim)
            free(tmp->heredoc_delim);
        if (tmp->heredoc_file)
        {
            unlink(tmp->heredoc_file);
            free(tmp->heredoc_file);
        }
        free(tmp);
    }
}

void	add_argument(t_cmd *cmd, char *arg)
{
    char	**new_args;
    int		i;

    if (!arg)
        return ;
    if (cmd->arg_count >= cmd->arg_capacity)
    {
        cmd->arg_capacity *= 2;
        new_args = (char **)malloc(sizeof(char *) * cmd->arg_capacity);
        if (!new_args)
            return ;
        i = 0;
        while (i < cmd->arg_count)
        {
            new_args[i] = cmd->args[i];
            i++;
        }
        while (i < cmd->arg_capacity)
            new_args[i++] = NULL;
        free(cmd->args);
        cmd->args = new_args;
    }
    cmd->args[cmd->arg_count++] = arg;
}

void builtin_echo(t_cmd *cmd)
{
    int i;
    int newline;

    i = 1;
    newline = 1;

    while (cmd->args[i] && is_valid_n_flag(cmd->args[i]))
    {
        newline = 0;
        i++;
    }
    while (cmd->args[i])
    {
        printf("%s", cmd->args[i]);
        if (cmd->args[i + 1])
            printf(" ");
        i++;
    }
    if (newline)
        printf("\n");
}

int is_valid_n_flag(const char *str)
{
    int i;
    i = 1;
    if (str[0] != '-' || str[1] != 'n')
        return 0;
    while (str[i])
    {
        if (str[i] != 'n')
            return 0;
        i++;
    }
    return 1;
}