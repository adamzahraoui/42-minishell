#include "../minishell.h"


void	print_cmds(t_cmd *cmd)
{
    int i;

    if (!cmd)
    {
        printf("No commands to print\n");
        return;
    }
    
    while (cmd)
    {
        printf("---- Command ----\n");
        printf("Args: ");
        i = 0;
        while (i < cmd->arg_count && cmd->args && cmd->args[i])
        {
            printf("[%s] ", cmd->args[i]);
            i++;
        }
        printf("\n");
        printf("Input file: %s\n", cmd->input_file ? cmd->input_file : "NULL");
        printf("Output file: %s\n", cmd->output_file ? cmd->output_file : "NULL");
        printf("Append output: %d\n", cmd->append_output);
        printf("Heredoc delim: %s\n", cmd->heredoc_delim ? cmd->heredoc_delim : "NULL");
        printf("------------------\n");

        cmd = cmd->next;
    }
}
void	process_commands_s(t_token **tokens, t_var *vars, char **env)
{
	t_cmd	*commands;
	t_cmd	*cur;

	commands = parse_commands(tokens, vars, env);
	if (commands)
	{
		// ✅ هنا تزيد طبع debug
		print_cmds(commands);

		cur = commands;
		while (cur)
		{
			if (cur->arg_count > 0 && ft_strncmp(cur->args[0], "echo", ft_strlen("echo")) == 0)
				builtin_echo(cur);
			// هنا ممكن تزيد checks إضافية
			cur = cur->next;
		}
		free_commands(commands);
	}
}

void	print_tokens(t_token **tokens, t_var *vars, char **env)
{
	t_token	*current;
	int		i;

	(void)vars;
	(void)env;
	current = *tokens;
	i = 1;
	printf("Tokens:\n");
	while (current)
	{
		printf("%d: [%s] (Type: %d)\n", i++, current->value, current->type);
		current = current->next;
	}
}

int	is_delimiter(char c)
{
	return (c == ' ' || c == '\t' || c == '|' || c == '<' || c == '>');
}

int	is_redirection(t_token_type type)
{
	return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_HEREDOC || type == TOKEN_APPEND);
}

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		free(tmp->value);
		free(tmp);
	}
}
