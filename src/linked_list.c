#include "linked_list.h"

/**
 * @brief Creates a new node with the given word.
 *
 * This function allocates memory for a new node, initializes it with the given word, and sets
 * the next pointer to NULL.
 *
 * @param word The word (data) to store in the new node.
 * @return A pointer to the newly created node.
 */
ListNode* create_node(Word word) {
    ListNode *new_node = (ListNode *)malloc(sizeof(ListNode));
    if (!new_node) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    new_node->word = word;
    new_node->next = NULL;
    return new_node;
}

/**
 * @brief Appends a new node with the given word to the end of the list.
 *
 * If the list is empty, the new node becomes the head. Otherwise, the new node is added to the end of the list.
 *
 * @param head A double pointer to the head of the list.
 * @param word The word (data) to store in the new node.
 */
void append_node(ListNode **head, Word word) {
    ListNode *new_node = create_node(word);
    if (*head == NULL) {
        *head = new_node;
    } else {
        ListNode *temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_node;
    }
}

/**
 * @brief Frees all the nodes in the linked list.
 *
 * This function traverses the linked list and frees the memory allocated for each node.
 *
 * @param head A pointer to the head of the list.
 */
void free_list(ListNode *head) {
    ListNode *temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}
