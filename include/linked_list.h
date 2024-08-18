#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdlib.h>
#include <stdio.h>

typedef unsigned short Word;

/**
 * @brief A structure representing a node in a singly linked list.
 *
 * Each node contains a word of data and a pointer to the next node in the list.
 */
typedef struct ListNode {
    Word word;              /**< The data stored in the node. */
    struct ListNode *next;  /**< Pointer to the next node in the list. */
} ListNode;

/**
 * @brief Creates a new node with the given word.
 *
 * @param word The word (data) to store in the new node.
 * @return A pointer to the newly created node.
 */
ListNode* create_node(Word word);

/**
 * @brief Appends a new node with the given word to the end of the list.
 *
 * @param head A double pointer to the head of the list.
 * @param word The word (data) to store in the new node.
 */
void append_node(ListNode **head, Word word);

/**
 * @brief Frees all the nodes in the linked list.
 *
 * @param head A pointer to the head of the list.
 */
void free_list(ListNode *head);

#endif /* LINKED_LIST_H */
