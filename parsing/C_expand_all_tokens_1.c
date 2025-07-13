#include "../minishell.h"

void	expand_all_tokens(t_token **tokens, t_var *vars, char **env)
{
	char	*expanded;
	t_token *tok;

	tok = *tokens;
	while (tok)
	{
		if (tok->type == TOKEN_WORD && tok->value)
		{
			expanded = expand_token(tok->value, vars, env);
			free(tok->value);
			tok->value = expanded;
		}
		tok = tok->next;
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

	len = 0;
	if (token[*i + 1] == '?')
		return (handle_exit_status(result));
	extract_var_name(token, i, var_name);
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

int	handle_exit_status(char *result)
{
	char	*var_value;
	int		len;

	var_value = ft_itoa(g_exit_status);
	if (!var_value)
		return (0);
	ft_strlcpy(result, var_value, MAX_TOKEN_LEN);
	len = ft_strlen(result);
	free(var_value);
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
