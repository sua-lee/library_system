#ifndef USERS_H
#define USERS_H

#include "../common/structures.h" // BookAndDate 등의 전방선언 또는 정의 포함

// Forward declarations
struct BookNode; // BookNode* reserved_book_node 때문
struct Queue;    // 만약 User 구조체 내에 Queue 포인터가 있다면

#define MAX_NAME_LEN 100
#define MAX_BORROW_LIMIT 3
#define MAX_BORROW_LIMIT_CLUB 5 // 도서 클럽 회원 대출 한도

// 사용자 구조체
typedef struct User {
    int number;                  // 사용자 번호
    char name[MAX_NAME_LEN];    // 사용자 이름
    struct User* next;          // 다음 사용자 포인터

    int overdue;
    int borrowed_num;
    struct BookNode* reserved_book_node;
    BookAndDate* borrowed_book_list_head; // common/structures.h 에 정의된 BookAndDate 사용
    BookAndDate* overdue_book_list_head;  // common/structures.h 에 정의된 BookAndDate 사용

    int is_book_club_member;     // 도서 클럽 가입 여부 (예: 0 = false, 1 = true)

} User;

// 함수 목록
User* create_user(int number, const char* name);
void insert_user(User** head, User* new_user);
void load_users(const char* filename, User** head);
void print_users(User* head);

User* find_user_by_number(User* head, int user_number);
User* find_user_by_name(User* head, const char* user_name);

#endif // USERS_H