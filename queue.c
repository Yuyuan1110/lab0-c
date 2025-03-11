#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

#include "queue.h"

/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *head =
        (struct list_head *) malloc(sizeof(struct list_head));
    if (!head) {
        return NULL;
    }

    INIT_LIST_HEAD(head);
    return head;
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    if (!head || list_empty(head)) {
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
    element->value = (char *) malloc(strlen(s) + 1);
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
    element->value = (char *) malloc(strlen(s) + 1);
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

    if (!head || list_is_singular(head) || list_empty(head)) {
        return false;
    }

    element_t *element, *safe = NULL;
    bool condition = false;
    list_for_each_entry_safe (element, safe, head, list) {
        if (strcmp(element->value, safe->value) == 0) {
            condition = true;
            list_del(&(element->list));
            q_release_element(element);
        } else if (condition) {
            condition = false;
            list_del(&(element->list));
            q_release_element(element);
        }
        if (safe->list.next == head) {
            break;
        }
    }

    if (condition) {
        struct list_head *last_node = head->prev;
        list_del(last_node);
        q_release_element(list_entry(last_node, element_t, list));
    }
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    if (!head || list_is_singular(head) || list_empty(head)) {
        return;
    }
    struct list_head *anode = NULL;
    struct list_head **pp = &(head->next);

    while (*pp != head && (*pp)->next != head) {
        anode = *pp;
        list_del(anode);
        list_add(anode, anode->next);
        pp = &(anode->next);
    }
    return;
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head || list_empty(head)) {
        return;
    }
    struct list_head *node = head;
    do {
        struct list_head *temp = node->next;
        node->next = node->prev;
        node->prev = temp;
        node = node->next;
    } while (node != head);
}

/* Reverse the nodes of the list k at a time */
// if k = 3;
// a <-> b <-> c <-> d <-> e <-> f  =>
// c <-> b <-> a <-> f <-> e <-> d
// a <-> b <-> c <-> d <-> e  =>
// c <-> b <-> a <-> d <-> e
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
    if (!head || list_empty(head)) {
        return;
    }
    if (k == 1) {
        q_reverse(head);
        return;
    }

    struct list_head *node = head->next;
    int size = q_size(node);
    while (size >= k) {
        struct list_head *tail;
        struct list_head *nhead = node;  // new head
        struct list_head *ngh = node;    // next group head

        for (int i = 0; i < k; i++) {
            ngh = ngh->next;
        }
        tail = ngh->prev;
        while (nhead != tail) {
            struct list_head *temp = nhead->next;
            list_del(nhead);
            list_add(nhead, tail);
            nhead = temp;
        }
        node = ngh;
        size -= k;
    }
}
void q_sort(struct list_head *head, bool descend)
{
    if (!head || list_empty(head) || list_is_singular(head)) {
        return;
    }

    struct list_head *tortoise = head->next;
    struct list_head *hare = head->next;

    while (hare->next != head && hare->next->next != head) {
        tortoise = tortoise->next;
        hare = hare->next->next;
    }

    struct list_head thead;
    struct list_head *head2 = &thead;
    list_cut_position(head2, head, tortoise);

    q_sort(head, descend);
    q_sort(head2, descend);

    struct list_head *curr1 = head->next;
    struct list_head *curr2 = head2->next;

    while (curr1 != head && curr2 != head2) {
        const element_t *val1 = list_entry(curr1, element_t, list);
        const element_t *val2 = list_entry(curr2, element_t, list);
        if (strcmp(val1->value, val2->value) > 0) {
            struct list_head *temp = curr2->next;
            list_move(curr2, curr1->prev);
            curr2 = temp;
        } else {
            curr1 = curr1->next;
        }
    }
    if (curr1 == head) {
        list_splice_tail(head2, head);
    }
    if (descend) {
        q_reverse(head);
    }
}
/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/

    if (!head || list_empty(head) || list_is_singular(head)) {
        return 0;
    }
    struct list_head *next = head->next;
    while (next->next != head) {
        const element_t *val1 = list_entry(next, element_t, list);
        element_t *val2 = list_entry(next->next, element_t, list);
        if (strcmp(val1->value, val2->value) > 0) {
            list_del(next->next);
            q_release_element(val2);
        } else {
            next = next->next;
        }
    }
    return q_size(head);
}

/* Remove every node which has a node with a strictly greater value anywhere
 * to the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    if (!head || list_empty(head) || list_is_singular(head)) {
        return 0;
    }
    struct list_head *prev = head->prev;
    while (prev->prev != head) {
        const element_t *val1 = list_entry(prev, element_t, list);
        element_t *val2 = list_entry(prev->prev, element_t, list);
        if (strcmp(val1->value, val2->value) > 0) {
            list_del(prev->prev);
            q_release_element(val2);
        } else {
            prev = prev->prev;
        }
    }
    return q_size(head);
}

/* Merge all the queues into one sorted queue, which is in
 * ascending/descending order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    if (!head || list_empty(head)) {
        return 0;
    }

    struct list_head *fqueue = list_entry(head->next, queue_contex_t, chain)->q;

    struct list_head *queue_ptr = head->next->next;
    while (queue_ptr != head) {
        struct list_head *queue =
            list_entry(queue_ptr, queue_contex_t, chain)->q;

        list_splice_init(queue, fqueue);
        list_entry(queue_ptr, queue_contex_t, chain)->size = 0;
        queue_ptr = queue_ptr->next;
    }
    q_sort(fqueue, descend);
    return q_size(fqueue);
}
