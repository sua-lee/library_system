#ifndef LIST_H
#define LIST_H

#include "structures.h" // BookAndDate 구조체 정의를 위함

// User 및 BookNode는 포인터로만 사용되므로 전방선언으로 충분할 수 있지만,
// 함수 시그니처에서 User* user_information 과 같이 사용되므로,
// 실제 User, BookNode 정의가 필요할 수 있음.
// 여기서는 BookAndDate 내의 User*와 BookNode*를 위해 structures.h에 전방선언된 것을 사용.
// 필요시 "../init-data/users.h" 와 "../init-data/book_tree.h" include.


// BookAndDate 리스트 관리 함수 프로토타입
void add_bad_node_to_list(BookAndDate** head, BookAndDate* new_node);
BookAndDate* find_bad_node_on_pickup_list(BookAndDate* head, struct User* user, struct BookNode* book);
BookAndDate* find_and_remove_bad_node_by_book(BookAndDate** head, struct BookNode* book_to_find);
int remove_bad_node_from_list_by_data(BookAndDate** head, BookAndDate* node_to_remove);
void free_bad_node(BookAndDate* node);
void free_bad_list(BookAndDate* head);

#endif // LIST_H