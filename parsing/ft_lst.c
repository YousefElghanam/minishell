#include "parsing.h"

// DELETE THIS FUNCTION, WE ONLY NEED IT FOR PRINTING THE TOKENS.
void	ft_lstiter(t_list *lst, void (*f)(t_print_d *), t_print_d *data)
{
	while (lst)
	{
		data->token = lst->token;
		f(data);
		lst = lst->next;
	}
}

t_list	*ft_lstnew(t_token *token)
{
	t_list	*node;

	node = malloc(sizeof(t_list));
	if (!node)
		return (NULL);
	node->token = token;
	node->next = NULL;
	return (node);
}

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*node;

	if (*lst)
	{
		node = ft_lstlast(*lst);
		node->next = new;
	}
	else
		*lst = new;
}

void	ft_lstdelone(t_list *lst, void (*del)(void *))
{
	del(lst->token);
	free(lst);
}

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*next;

	while (*lst)
	{
		next = (*lst)->next;
		ft_lstdelone(*lst, del);
		*lst = next;
	}
	*lst = NULL;
}

t_list	*ft_lstlast(t_list *lst)
{
	while (lst)
	{
		if (!lst->next)
			return (lst);
		lst = lst->next;
	}
	return (lst);
}
