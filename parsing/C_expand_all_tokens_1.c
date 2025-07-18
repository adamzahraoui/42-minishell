#include "../minishell.h"

void	expand_all_tokens(t_token **tokens, t_var *vars, char **env)
{
    char	*expanded;
    t_token *tok;
    t_token *prev;
    t_token *next;

    tok = *tokens;
    prev = NULL;
    while (tok)
    {
        next = tok->next;
        if (tok->type == TOKEN_WORD && tok->value && 
            !(prev && prev->type == TOKEN_HEREDOC))
        {
            expanded = expand_token(tok->value, vars, env);
            if (expanded && *expanded)
            {
                free(tok->value);
                tok->value = expanded;
                prev = tok;
            }
            else
            {
                if (prev)
                    prev->next = next;
                else
                    *tokens = next;
                free(tok->value);
                free(expanded);
                free(tok);
                tok = next;
                continue;
            }
        }
        else
        {
            prev = tok;
        }
        tok = next;
    }
}

char	*expand_token(char *token, t_var *vars, char **env)
{
	char				*result;
	t_token_state		st;
	t_expand_context	ctx;

	ctx.vars = vars;
	ctx.env = env;
	memset(&st, 0, sizeof(st));
	result = malloc(MAX_TOKEN_LEN);
	if (!result)
		return (NULL);
	while (token[st.i] && st.j < MAX_TOKEN_LEN - 1)
	{
		if (token[st.i] == '\'' && !st.in_double)
			st.in_single = !st.in_single;
		else if (token[st.i] == '\"' && !st.in_single)
			st.in_double = !st.in_double;
		else if (token[st.i] == '$' && !st.in_single)
			st.j += expand_variable(token, &(st.i), result + st.j, &ctx);
		else
			result[st.j++] = token[st.i];
		st.i++;
	}
	result[st.j] = '\0';
	return (result);
}

int	expand_variable(char *token, int *i, char *result, t_expand_context *ctx)
{
    char	var_name[256];
    int		len;
    char	*val;
    int		var_name_len;

    len = 0;
    var_name_len = extract_var_name(token, i, var_name);
        if (var_name_len == 0)
    {
        result[0] = '$';
        return (1);
    }
    val = get_shell_var(ctx->vars, var_name);
    if (!val)
        val = get_env_value(ctx->env, var_name);
    if (val)
    {
        len = ft_strlen(val);
        ft_strlcpy(result, val, MAX_TOKEN_LEN);
    }
    else
        result[0] = '\0';
    return (len);
}

int	extract_var_name(const char *token, int *i, char *var_name)
{
	int	j;

	j = 0;
	(*i)++;
	while (token[*i] && (ft_isalnum(token[*i]) || token[*i] == '_') && j < 255)
		var_name[j++] = token[(*i)++];
	var_name[j] = '\0';
	(*i)--;
	return (j);
}

char **convert_myenv_to_env(t_myenv *myenv)
{
    t_myenv *temp;
    char **env;
    int count = 0;
    int i = 0;

    temp = myenv;
    while (temp)
    {
        count++;
        temp = temp->next;
    }
    env = malloc(sizeof(char*) * (count + 1));
    if (!env)
        return NULL;

    temp = myenv;
    while (temp && i < count)
    {
        env[i] = ft_strdup(temp->data);
        temp = temp->next;
        i++;
    }
    env[i] = NULL;
    return env;
}

char	*get_myenv_value(t_myenv *myenv, char *name)
{
    int name_len;
    
    if (!myenv || !name)
        return NULL;
    
    name_len = ft_strlen(name);
    
    while (myenv)
    {
        if (ft_strncmp(myenv->data, name, name_len) == 0 && 
            myenv->data[name_len] == '=')
            return (myenv->data + name_len + 1);
        myenv = myenv->next;
    }
    return NULL;
}