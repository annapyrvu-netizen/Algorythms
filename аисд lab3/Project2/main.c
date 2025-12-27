#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#define EPS 1e-6

typedef struct _node node_t;

struct _node
{
    double value;
    node_t* next;
};

node_t* list_create()
{
    node_t* node = malloc(sizeof(node_t));

    node->next = NULL;

    return node;
}

void list_add(node_t** list, double value)
{
    node_t* node = list_create();
    node->value = value;

    if (*list == NULL)
    {
        *list = node;
        return;
    }

    node_t* tail = *list;

    while (tail->next != NULL)
    {
        tail = tail->next;
    }

    tail->next = node;
}

void list_print(node_t* list)
{
    while (list != NULL)
    {
        printf("(%p, %lf) -> %p\n", list, list->value, list->next);
        list = list->next;
    }
    printf("NULL\n");
}

void list_free(node_t** list)
{
    if (*list == NULL)
        return;

    node_t* next = (*list)->next;

    while (*list != NULL)
    {
        free(*list);

        *list = next;

        if (*list != NULL)
            next = (*list)->next;
    }

    *list = NULL;
}

bool double_equal(double a, double b)
{
    return fabs(a - b) < EPS;
}

void list_delete(node_t** list, double to_delete)
{
    while (*list != NULL && double_equal((*list)->value, to_delete))
    {
        node_t* tmp = (*list)->next;
        free(*list);
        *list = tmp;
    }

    if (*list == NULL)
        return;

    node_t* prev = *list;
    node_t* cur = prev->next;
    while (cur != NULL)
    {
        if (double_equal(cur->value, to_delete))
        {
            prev->next = cur->next;
            free(cur);
            cur = prev->next;
        }
        else
        {
            prev = cur;
            cur = cur->next;
        }
    }
}

double list_max(node_t* list)
{
    double m = list->value;
    while (list != NULL)
    {
        if (list->value > m)
        {
            m = list->value;
        }
        list = list->next;
    }

    return m;
}

int main()
{
    srand(time(NULL));

    node_t* list = NULL;

    for (int i = 0; i < 10; i++)
    {
        list_add(&list, rand() % 10);
    }

    list_print(list);

    double max_elem = list_max(list);

    list_delete(&list, max_elem);

    printf("max_elem: %lf\n", max_elem);

    list_print(list);

    list_free(&list);
}