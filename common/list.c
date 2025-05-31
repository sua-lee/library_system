#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include "structures.h" // BookAndDate 정의
// BookAndDate 구조체 내 User* 와 BookNode* 멤버를 사용하기 위해 실제 정의 필요
#include "../init_data/users.h"
#include "../init_data/book_tree.h"


void add_bad_node_to_list(BookAndDate** head, BookAndDate* new_node) {
    if (!new_node) return;
    new_node->next = *head;
    *head = new_node;
}

BookAndDate* find_bad_node_on_pickup_list(BookAndDate* head, User* user, BookNode* book) {
    BookAndDate* current = head;
    while (current) {
        if (current->user_information == user && current->book_information == book) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

BookAndDate* find_and_remove_bad_node_by_book(BookAndDate** head, BookNode* book_to_find) {
    if (!head || !*head || !book_to_find) return NULL;
    BookAndDate* current = *head;
    BookAndDate* prev = NULL;
    while (current) {
        if (current->book_information == book_to_find) {
            if (prev == NULL) {
                *head = current->next;
            } else {
                prev->next = current->next;
            }
            current->next = NULL;
            return current;
        }
        prev = current;
        current = current->next;
    }
    return NULL;
}

int remove_bad_node_from_list_by_data(BookAndDate** head, BookAndDate* node_to_remove) {
    if (!head || !*head || !node_to_remove) return 0;
    BookAndDate* current = *head;
    BookAndDate* prev = NULL;

    while(current) {
        if (current == node_to_remove) {
            if (prev == NULL) {
                *head = current->next;
            } else {
                prev->next = current->next;
            }
            return 1;
        }
        prev = current;
        current = current->next;
    }
    return 0;
}

void free_bad_node(BookAndDate* node) {
    free(node);
}

void free_bad_list(BookAndDate* head) {
    BookAndDate* current = head;
    BookAndDate* next_node;
    while (current != NULL) {
        next_node = current->next;
        free(current);
        current = next_node;
    }
}