#ifndef LIST_H
#define LIST_H

#include "structures.h" // BookAndDate 구조체 정의를 위함

// BookAndDate 리스트 관리 함수 프로토타입
void add_bad_node_to_list(BookAndDate** head, BookAndDate* new_node);
BookAndDate* find_bad_node_on_pickup_list(BookAndDate* head, struct User* user, struct BookNode* book);
BookAndDate* find_and_remove_bad_node_by_book(BookAndDate** head, struct BookNode* book_to_find);
int remove_bad_node_from_list_by_data(BookAndDate** head, BookAndDate* node_to_remove);
void free_bad_node(BookAndDate* node);
void free_bad_list(BookAndDate* head);

#endif // LIST_H