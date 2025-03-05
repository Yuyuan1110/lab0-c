#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

#include "queue.h"

/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *head = malloc(sizeof(struct list_head));
    if (!head) {
        return NULL;
    }

    INIT_LIST_HEAD(head);
    return head;
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    if (!head) {
        return;
    }

    element_t *entry, *safe = NULL;

    list_for_each_entry_safe (entry, safe, head, list) {
        list_del(&entry->list);
        q_release_element(entry);
    }
    free(head);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head) {
        head = q_new();
    }
    element_t *element = (element_t *) malloc(sizeof(element_t));
    const struct list_head *list =
        (struct list_head *) malloc(sizeof(struct list_head));
    element->value = malloc(strlen(s) + 1);
    if (!element || !list || !element->value) {
        return false;
    }
    strncpy(element->value, s, strlen(s) + 1);
    element->list = *list;

    list_add(&element->list, head);

    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head) {
        head = q_new();
    }
    element_t *element = (element_t *) malloc(sizeof(element_t));
    const struct list_head *list =
        (struct list_head *) malloc(sizeof(struct list_head));
    element->value = malloc(strlen(s) + 1);
    if (!element || !list) {
        return false;
    }
    strncpy(element->value, s, strlen(s) + 1);
    element->list = *list;

    list_add_tail(&element->list, head);

    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head)) {
        return NULL;
    }

    element_t *element = list_first_entry(head, element_t, list);
    list_del(&element->list);

    if (sp != NULL && bufsize > 0) {
        strncpy(sp, element->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }
    return element;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head)) {
        return NULL;
    }

    element_t *element = list_last_entry(head, element_t, list);
    list_del(&element->list);

    if (sp != NULL && bufsize > 0) {
        strncpy(sp, element->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }
    return element;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head)
        return 0;

    int len = 0;
    struct list_head *li;

    list_for_each (li, head)
        len++;
    return len;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    // Eliminate the NULL, empty and singular element(head)
    if (!head || list_is_singular(head) || list_empty(head)) {
        return false;
    }

    // use hare-tortoies pointer to find the middle element,
    // tortoise take 1 step and hare take 2 step,
    // when hare arraive the destination(or bake to head),
    // tortoise just went half of journy.
    struct list_head *hare = head;
    struct list_head *tortoise = head;
    do {
        hare = hare->next->next;
        tortoise = tortoise->next;
    } while (hare != head && hare->next != head);
    element_t *element = list_entry(tortoise, element_t, list);
    list_del(&element->list);
    q_release_element(element);

    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    if (!head || list_is_singular(head) || list_empty(head)) {
        return;
    }
    struct list_head **pp = &(head->next);
    struct list_head *a, *b, *temp = head;

    while (*pp != head && (*pp)->next != head) {
        a = *pp;
        b = a->next;
        a->next = b->next;
        b->next = a;
        b->prev = temp;
        a->prev = b;
        *pp = b;
        temp = a;
        pp = &(a->next);
    }
    (*pp)->prev = temp;
    return;
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head) {}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend) {}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
