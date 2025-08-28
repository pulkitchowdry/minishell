/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 16:09:08 by chikoh            #+#    #+#             */
/*   Updated: 2025/08/28 13:27:24 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"

extern int	g_ret_code;

t_list	*construct_single_quote(char **string)
{
	char	*string_start;
	t_token	*result;

	string_start = *string;
	(*string)++;
	while (**string && **string != '\'')
	{
		if (**string == '\\')
			(*string)++;
		(*string)++;
	}
	(*string)++;
	result = (t_token *)ft_calloc(sizeof(t_token), 1);
	result->type = SINGLE_QUOTE_STRING;
	result->string = (char *)ft_calloc(sizeof(char),
			*string - string_start + 1);
	ft_strlcpy(result->string, string_start,
		*string - string_start + 1);
	return (ft_lstnew(result));
}

t_list	*construct_double_quote(char **string)
{
	char	*string_start;
	t_token	*result;

	string_start = *string;
	(*string)++;
	while (**string && **string != '"')
	{
		if (**string == '\\')
			(*string)++;
		(*string)++;
	}
	(*string)++;
	result = (t_token *)ft_calloc(sizeof(t_token), 1);
	result->type = DOUBLE_QUOTE_STRING;
	result->string = (char *)ft_calloc(sizeof(char),
			*string - string_start + 1);
	ft_strlcpy(result->string, string_start,
		*string - string_start + 1);
	return (ft_lstnew(result));
}

t_list	*construct_2_characters(char **string)
{
	char	*string_start;
	t_token	*result;

	string_start = *string;
	result = (t_token *)ft_calloc(sizeof(t_token), 1);
	if (**string == '<' && *(*string + 1) == '<')
		result->type = HERE_DOC;
	else if (**string == '>' && *(*string + 1) == '>')
		result->type = REDIRECT_APPEND;
	else if (**string == '|' && *(*string + 1) == '|')
		result->type = LOGICAL_OR;
	else if (**string == '&' && *(*string + 1) == '&')
		result->type = LOGICAL_AND;
	*string += 2;
	result->string = (char *)ft_calloc(sizeof(char),
			*string - string_start + 1);
	ft_strlcpy(result->string, string_start,
		*string - string_start + 1);
	return (ft_lstnew(result));
}

t_list	*construct_1_character(char **string)
{
	char	*string_start;
	t_token	*result;

	string_start = *string;
	result = (t_token *)ft_calloc(sizeof(t_token), 1);
	if (**string == '>' && *(*string + 1) != '>')
		result->type = REDIRECT_OUTPUT;
	else if (**string == '<' && *(*string + 1) != '<')
		result->type = REDIRECT_INPUT;
	else if (**string == '|' && *(*string + 1) != '|')
		result->type = PIPE;
	else if (**string == '(')
		result->type = OPEN_BRACKET;
	else if (**string == ')')
		result->type = CLOSE_BRACKET;
	else if (**string == '=')
		result->type = ASSIGNMENT;
	*string += 1;
	result->string = (char *)ft_calloc(sizeof(char),
			*string - string_start + 1);
	ft_strlcpy(result->string, string_start,
		*string - string_start + 1);
	return (ft_lstnew(result));
}

t_list	*construct_string(char **string)
{
	char	*string_start;
	t_token	*result;

	result = (t_token *)ft_calloc(sizeof(t_token), 1);
	result->type = STRING;
	string_start = *string;
	while (**string && (**string != '\'') && (**string != '"') && (**string != '>')
		&& (**string != '<') && (**string != '|') && (**string != '&')
		&& (**string != ' ') && (**string != '$' || **string != '?') && (**string != ')')
		&& (**string != '=') && (**string != '('))
		(*string)++;
	result->string = (char *)ft_calloc(sizeof(char),
			*string - string_start + 1);
	ft_strlcpy(result->string, string_start,
		*string - string_start + 1);
	return (ft_lstnew(result));
}

t_list	*construct_space(char **string)
{
	char	*string_start;
	t_token	*result;

	result = (t_token *)ft_calloc(sizeof(t_token), 1);
	result->type = SPACES;
	string_start = *string;
	while (**string && **string == ' ')
		(*string)++;
	result->string = (char *)ft_calloc(sizeof(char),
			*string - string_start + 1);
	ft_strlcpy(result->string, string_start,
		*string - string_start + 1);
	return (ft_lstnew(result));
}

t_list	*construct_variable(char **string)
{
	char	*string_start;
	t_token	*result;
	char	curly_brackets;

	result = (t_token *)ft_calloc(sizeof(t_token), 1);
	result->type = VARIABLE;
	string_start = *string;
	(*string)++;
	// if (**string == '?')
	// 	(*string)++;
	if (**string)
	{
		curly_brackets = **string == '{';
		*string += curly_brackets;
		while (**string && (ft_isalnum(**string) || **string == '_'
				|| (curly_brackets && **string == '}') || **string == '?'))
			(*string)++;
	}
	result->string = (char *)ft_calloc(sizeof(char),
			*string - string_start + 1);
	ft_strlcpy(result->string, string_start,
		*string - string_start + 1);
	return (ft_lstnew(result));
}

void	free_token(void *token)
{
	free(((t_token *)token)->string);
	free(token);
}

t_list	*create_tokens(char *string)
{
	t_list	*result;

	result = 0;
	while (*string)
	{
		if (*string == '\'')
			ft_lstadd_back(&result, construct_single_quote(&string));
		else if (*string == '"')
			ft_lstadd_back(&result, construct_double_quote(&string));
		else if (*string == '>' && *(string + 1) != '>')
			ft_lstadd_back(&result, construct_1_character(&string));
		else if (*string == '<' && *(string + 1) != '<')
			ft_lstadd_back(&result, construct_1_character(&string));
		else if (*string == '|' && *(string + 1) != '|')
			ft_lstadd_back(&result, construct_1_character(&string));
		else if (*string == '(')
			ft_lstadd_back(&result, construct_1_character(&string));
		else if (*string == ')')
			ft_lstadd_back(&result, construct_1_character(&string));
		else
			process_additional_tokens(&result, &string);
	}
	return (result);
}

void	process_additional_tokens(t_list **result, char **string)
{
	if (**string == '>' && *(*string + 1) == '>')
		ft_lstadd_back(result, construct_2_characters(string));
	else if (**string == '<' && *(*string + 1) == '<')
		ft_lstadd_back(result, construct_2_characters(string));
	else if (**string == '|' && *(*string + 1) == '|')
		ft_lstadd_back(result, construct_2_characters(string));
	else if (**string == '&' && *(*string + 1) == '&')
		ft_lstadd_back(result, construct_2_characters(string));
	else if (**string == ' ')
		ft_lstadd_back(result, construct_space(string));
	else if (**string == '=')
		ft_lstadd_back(result, construct_1_character(string));
	else if (**string == '$' && (ft_isalnum(*(*string + 1))
			|| *(*string + 1) == '_' || *(*string + 1) == '{'
			|| *(*string + 1) == '?'))
		ft_lstadd_back(result, construct_variable(string));
	else
		ft_lstadd_back(result, construct_string(string));
}

char	is_wildcard_present(char *string)
{
	while (*string)
	{
		if (*string == '*')
			return (1);
		string++;
	}
	return (0);
}
