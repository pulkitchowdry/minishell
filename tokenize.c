/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chikoh <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 16:09:08 by chikoh            #+#    #+#             */
/*   Updated: 2025/08/04 17:21:39 by chikoh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"

t_list	*construct_single_quote(char **string)
{
	char	*string_start;
	char	*result;

	string_start = *string;
	(*string)++;
	while (**string != '\'' && **string)
	{
		if (**string == '\\')
			(*string)++;
		(*string)++;
	}
	(*string)++;
	result = (char *)ft_calloc(sizeof(char), *string - string_start + 1);
	ft_strlcpy(result, string_start, *string - string_start + 1);
	return (ft_lstnew(result));
}

t_list	*construct_double_quote(char **string)
{
	char	*string_start;
	char	*result;

	string_start = *string;
	(*string)++;
	while (**string != '"' && **string)
	{
		if (**string == '\\')
			(*string)++;
		(*string)++;
	}
	(*string)++;
	result = (char *)ft_calloc(sizeof(char), *string - string_start + 1);
	ft_strlcpy(result, string_start, *string - string_start + 1);
	return (ft_lstnew(result));
}

t_list	*construct_2_characters(char **string)
{
	char	*string_start;
	char	*result;

	string_start = *string;
	*string += 2;
	result = (char *)ft_calloc(sizeof(char), *string - string_start + 1);
	ft_strlcpy(result, string_start, *string - string_start + 1);
	return (ft_lstnew(result));
}

t_list	*construct_1_character(char **string)
{
	char	*string_start;
	char	*result;

	string_start = *string;
	*string += 1;
	result = (char *)ft_calloc(sizeof(char), *string - string_start + 1);
	ft_strlcpy(result, string_start, *string - string_start + 1);
	return (ft_lstnew(result));
}

t_list	*construct_string(char **string)
{
	char	*string_start;
	char	*result;

	string_start = *string;
	while ((**string != '\'') && (**string != '"') && (**string != '>')
			&& (**string != '<') && (**string != '|') && (**string != '&')
			&& (**string != ' ') && (**string != '$') && **string)
		(*string)++;
	result = (char *)ft_calloc(sizeof(char), *string - string_start + 1);
	ft_strlcpy(result, string_start, *string - string_start + 1);
	return (ft_lstnew(result));
}

t_list	*construct_space(char **string)
{
	char	*string_start;
	char	*result;

	string_start = *string;
	while (**string == ' ' && **string)
		(*string)++;
	result = (char *)ft_calloc(sizeof(char), *string - string_start + 1);
	ft_strlcpy(result, string_start, *string - string_start + 1);
	return (ft_lstnew(result));
}

t_list	*construct_variable(char **string)
{
	char	*string_start;
	char	*result;
	char	curly_brackets;

	string_start = *string;
	(*string)++;
	if (**string == '?')
	{
		(*string)++;
		result = (char *)ft_calloc(sizeof(char), *string - string_start + 1);
		ft_strlcpy(result, string_start, *string - string_start + 1);
	}
	else
	{
		curly_brackets = **string == '{';
		*string += curly_brackets;
		while ((ft_isalnum(**string) || **string == '_' || (curly_brackets && **string == '}')) && **string)
			(*string)++;
		result = (char *)ft_calloc(sizeof(char), *string - string_start + 1);
		ft_strlcpy(result, string_start, *string - string_start + 1);
	}
	return (ft_lstnew(result));
}

t_list	*create_tokens(char *string)
{
	char	*string_start;
	t_list	*result;

	string_start = string;
	result = 0;
	while(*string)
	{
		if (*string == '\'')
			ft_lstadd_back(&result, construct_single_quote(&string));
		else if (*string == '"')
			ft_lstadd_back(&result, construct_double_quote(&string));
		else if (*string == '>' && *(string + 1) == '>')
			ft_lstadd_back(&result, construct_2_characters(&string));
		else if (*string == '>' && *(string + 1) != '>')
			ft_lstadd_back(&result, construct_1_character(&string));
		else if (*string == '<' && *(string + 1) == '<')
			ft_lstadd_back(&result, construct_2_characters(&string));
		else if (*string == '<' && *(string + 1) != '<')
			ft_lstadd_back(&result, construct_1_character(&string));
		else if (*string == '|' && *(string + 1) != '|')
			ft_lstadd_back(&result, construct_1_character(&string));
		else if (*string == '|' && *(string + 1) == '|')
			ft_lstadd_back(&result, construct_2_characters(&string));
		else if (*string == '&' && *(string + 1) == '&')
			ft_lstadd_back(&result, construct_2_characters(&string));
		else if (*string == '(')
			ft_lstadd_back(&result, construct_1_character(&string));
		else if (*string == ')')
			ft_lstadd_back(&result, construct_1_character(&string));
		else if (*string == ' ')
			ft_lstadd_back(&result, construct_space(&string));
		else if (*string == '$' && (ft_isalnum(*(string + 1)) || *(string + 1) == '_'))
			ft_lstadd_back(&result, construct_variable(&string));
		else
			ft_lstadd_back(&result, construct_string(&string));
	}
	return (result);
}

