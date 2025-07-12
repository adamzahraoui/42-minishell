#include "../minishell.h"

char	*get_next_line(int fd)
{
    char	*line;
    char	*result;

    if (fd == STDIN_FILENO)
    {
        line = readline("> ");
        if (!line)
            return (NULL);
        result = ft_strjoin(line, "\n");
        free(line);
        return (result);
    }
    else
    {
        char	*buffer;
        char	c;
        int		i;
        int		bytes_read;

        buffer = malloc(1000);
        if (!buffer)
            return (NULL);
        i = 0;
        while ((bytes_read = read(fd, &c, 1)) > 0)
        {
            if (c == '\n')
                break ;
            buffer[i++] = c;
        }
        buffer[i] = '\0';
        if (bytes_read <= 0 && i == 0)
            return (free(buffer), NULL);
        return (buffer);
    }
}