#include "../minishell.h"


int	is_valid_n_flag(const char *str)
{
	int	i;

	i = 1;
	if (str[0] != '-' || str[1] != 'n')
		return (0);
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

void	ft_echo(t_cmd *cmd, t_myenv **myenv)
{
	int	i;
	int	newline;
    t_myenv *env;
    int j;

	i = 1;
    env = *myenv;
	newline = 1;
	while (cmd->args[i] && is_valid_n_flag(cmd->args[i]))
	{
		newline = 0;
		i++;
	}
	while (cmd->args[i])
	{
        j = 0;
        while(cmd->args[i][j])
        {
            if(ft_strncmp(&cmd->args[i][j], "$?", ft_strlen("$?")) == 0)
            {
                printf("%d", env->i);
                j += 2;
            }
            printf("%c", cmd->args[i][j]);
            j++;
        } 
		if (cmd->args[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
    set_status(myenv, NULL, 0);
}
