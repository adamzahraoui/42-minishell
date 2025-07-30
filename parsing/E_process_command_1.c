#include "../minishell.h"

void process_commands(t_token **tokens, t_var *vars, char **env, t_cmd **cmd)
{
    t_cmd *cur;

    *cmd = parse_commands(tokens, vars, env);
    if (*cmd)
    {
        cur = *cmd;
        while (cur)
        {
            if (cur->arg_count == 0 && (cur->input_file ))
            {
                if (cur->input_file)
                    printf("minishell: %s: No such file or directory\n", cur->input_file);
                free_commands(*cmd);
                *cmd = NULL;
                return;
            }

            else if (cur->arg_count > 0)
            {
                // if (ft_strchr(cur->args[0], '/'))
                // {
                //     printf("minishell: %s: No such file or directory\n", cur->args[0]);
                //     free_commands(*cmd);
                //     *cmd = NULL;
                //     return;
                // }
                if (strcmp(cur->args[0], "echo") == 0)
                    builtin_echo(cur);
            }
            cur = cur->next;
        }
    }
}

t_cmd *parse_commands(t_token **tokens, t_var *vars, char **env)
{
    t_cmd *head;
    t_cmd *current;
    t_token *token_ptr;

    head = NULL;
    current = NULL;
    token_ptr = *tokens;
    while (token_ptr)
    {
        if (!head)
        {
            head = parse_command(&token_ptr, vars, env);
            current = head;
        }
        else
        {
            current->next = parse_command(&token_ptr, vars, env);
            current = current->next;
        }
        if (token_ptr && token_ptr->type == TOKEN_PIPE)
            token_ptr = token_ptr->next;
    }
    return (head);
}

t_cmd *parse_command(t_token **tokens, t_var *vars, char **env)
{
    t_cmd *cmd;
    t_token *token_ptr;

    (void)vars;
    (void)env;
    cmd = init_command();
    if (!cmd)
        return (NULL);
    token_ptr = *tokens;
    while (token_ptr && token_ptr->type != TOKEN_PIPE)
    {
        if (is_redirection(token_ptr->type))
        {
            if (!handle_redirection(cmd, &token_ptr, tokens, vars, env))
                return (NULL);
            continue;
        }
        add_argument(cmd, ft_strdup(token_ptr->value));
        token_ptr = token_ptr->next;
    }
    *tokens = token_ptr;
    return (cmd);
}

t_cmd *init_command(void)
{
    t_cmd *cmd;
    int i;

    cmd = (t_cmd *)malloc(sizeof(t_cmd));
    if (!cmd)
        return (NULL);
    cmd->args = NULL;
    cmd->arg_count = 0;
    cmd->arg_capacity = 10;
    cmd->input_file = NULL;
    cmd->output_file = NULL;
    cmd->append_output = 0;
    cmd->heredoc_delim = NULL;
    cmd->heredoc_file = NULL;
    cmd->saved_stdin = -1;
    cmd->next = NULL;
    cmd->args = (char **)malloc(sizeof(char *) * cmd->arg_capacity);
    if (!cmd->args)
        return (free(cmd), NULL);
    i = 0;
    while (i < cmd->arg_capacity)
    {
        cmd->args[i] = NULL;
        i++;
    }
    return (cmd);
}

int handle_redirection(t_cmd *cmd, t_token **token_ptr, t_token **tokens, t_var *vars, char **env)
{
    t_token_type type;
    t_token *next;

    type = (*token_ptr)->type;
    next = (*token_ptr)->next;
    if (!next || next->type != TOKEN_WORD)
    {
        ft_putendl_fd("Error: Invalid redirection", 2);
        free_commands(cmd);
        *tokens = NULL;
        return (0);
    }
    if (!dispatch_redirection(cmd, next, type, vars, env))
    {
        free_commands(cmd);
        *tokens = NULL;
        return (0);
    }
    *token_ptr = next->next;
    return (1);
}