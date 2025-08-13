#include "../minishell.h"


void	setup_signals(void)
{
    signal(SIGINT, handle_sigint);
    signal(SIGQUIT, SIG_IGN);
}

void	handle_sigint(int sig)
{
    (void)sig;
    write(1, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

int validate_syntax(t_token *tokens)
{
    t_token *current = tokens;
    
    if (current && current->type == TOKEN_PIPE)
    {
        ft_putendl_fd("bash: syntax error near unexpected token `|'", 2);
        return 0;
    }
    while (current)
    {
        if (is_redirection(current->type))
        {
            t_token *next = current->next;
            if (!next)
            {
                ft_putendl_fd("bash: syntax error near unexpected token `newline'", 2);
                return 0;
            }
            if (next->type == TOKEN_PIPE)
            {
                ft_putendl_fd("bash: syntax error near unexpected token `|'", 2);
                return 0;
            }
            if (is_redirection(next->type))
            {
                ft_putstr_fd("bash: syntax error near unexpected token `", 2);
                ft_putstr_fd(next->value, 2);
                ft_putendl_fd("'", 2);
                return 0;
            }
        }
        else if (current->type == TOKEN_PIPE)
        {
            t_token *next = current->next;
            if (!next)
            {
                ft_putendl_fd("bash: syntax error near unexpected token `newline'", 2);
                return 0;
            }
            if (next->type == TOKEN_PIPE)
            {
                ft_putendl_fd("bash: syntax error near unexpected token `|'", 2);
                return 0;
            }
        }
        current = current->next;
    }
    
    return 1;
}

void handle_command(char *input, t_expand_context *ctx, t_cmd **cmd, t_token **tokens, t_myenv **myenv)
{
    char *trimmed;
    (void)myenv;

    trimmed = ft_strtrim(input, " \t");
    if (trimmed && *trimmed)
        add_history(trimmed);
    free(trimmed);
    (*tokens) = tokenize(input);
    if (*tokens)
    {
        ctx->env = convert_myenv_to_env(*myenv);
        if (!validate_syntax(*tokens))
        {
            free_tokens(*tokens);
            free_env_array(ctx->env);
            return;
        }
        expand_all_tokens(tokens, ctx );
        process_commands(tokens, ctx->vars, ctx->env, cmd);
        free_env_array(ctx->env);
    }
    free_tokens(*tokens);
    free(input);
}

int	main(int argc, char **argv, char **env)
{
    char	*input;
    t_cmd	*cmd;
    t_token	*tokens;
    t_myenv *myenv;
    t_myenv_ex *myenv_ex;
    t_expand_context ctx;

    cmd = NULL;
    tokens = NULL;
    myenv = NULL;
    myenv_ex = NULL;
    ctx.vars = NULL;
    declare_env(&myenv, &myenv_ex, env);
    add_back_env(&myenv, "?=0");
    myenv->i = 0;
    (void)argc;
    (void)argv;
    setup_signals();
    while (1)
    {
        input = readline("minishell> ");
        if (!input)
            ft_free_error("exit\n", &myenv, &myenv_ex, 139);
        handle_command(input, &ctx, &cmd, &tokens, &myenv);
        if (cmd != NULL)
        {
            cmd_ex(&cmd, env, &myenv, &myenv_ex);
            ft_free_all();
            /* cmd_ex now handles freeing the commands */
        }
        /* Reset tokens to NULL since they're freed in handle_command */
        tokens = NULL;
        
    }
    rl_clear_history();
    return (0);
}
