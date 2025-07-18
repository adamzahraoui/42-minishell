#include "../minishell.h"


void	setup_signals(void)
{
    signal(SIGINT, handle_sigint);
}

void	handle_sigint(int sig)
{
    (void)sig;
    write(1, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

void	handle_command(char *input, char **env, t_var **vars, t_cmd **cmd, t_token **tokens, t_myenv **myenv)
{
    char	*trimmed;
    char    **current_env;
    (void)env;


    trimmed = ft_strtrim(input, " \t");
    if (trimmed && *trimmed)
        add_history(trimmed);
    free(trimmed);
    if (ft_strncmp(input, "exit", 5) == 0)
    {
        free(input);
        exit(0);
    }
    (*tokens) = tokenize(input);
    if (*tokens)
    {
        current_env = convert_myenv_to_env(*myenv);
        expand_all_tokens(tokens, *vars, current_env);
        process_commands(tokens, *vars, current_env, cmd);
                free(current_env);
    }
    free_tokens(*tokens);
    free(input);
}

int	main(int argc, char **argv, char **env)
{
    char	*input;
    t_var	*vars;
    t_cmd	*cmd;
    t_token	*tokens;
    t_myenv *myenv;
    t_myenv_ex *myenv_ex;

    vars = NULL;
    cmd = NULL;
    tokens = NULL;
    myenv = NULL;
    myenv_ex = NULL;
    declare_env(&myenv, &myenv_ex, env);
    (void)argc;
    (void)argv;
    setup_signals();
    while (1)
    {
        input = readline("minishell> ");
        if (!input)
        {
            printf("exit\n");
            exit(139);
        }
        if (handle_assignment_or_empty(input, &vars, env))
            continue ;
        handle_command(input, env, &vars, &cmd, &tokens, &myenv);
        cmd_ex(&cmd, &tokens, env, &myenv, &myenv_ex);
    }
    clear_history();
    return (0);
}