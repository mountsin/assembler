#include "linked_list.h"
#include <stdlib.h>

struct list
{
	void *data;
	struct list *next;
};

ListPtr create_list(void)
{
	ListPtr list = (ListPtr)malloc(sizeof(ListPtr));
	return list;
}

ListPtr list_add(ListPtr list, void *data)
{
	ListPtr tmp;
	ListPtr new_rec;

	if(NULL != list)
		for(tmp = list; NULL != tmp->next; tmp = tmp->next);
	else
		tmp = NULL;

	new_rec = (ListPtr)malloc(sizeof(ListPtr));
	if(NULL != new_rec)
	{
		new_rec->next = NULL;
		new_rec->data = data;
		if(NULL != list)
			tmp->next = new_rec;
	}
	return new_rec;
}

void list_dispose(ListPtr list)
{
	ListPtr next;
	ListPtr current;

	for(current = list; NULL != current;)
	{
		next = current->next;
		free(current);
		current = next;
	}
}

