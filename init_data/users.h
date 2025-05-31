#ifndef USERS_H
#define USERS_H

#define MAX_NAME_LEN 100

// 사용자 구조체
typedef struct User {
    int number;                  // 사용자 번호
    char name[MAX_NAME_LEN];    // 사용자 이름
    struct User* next;          // 다음 사용자 포인터
} User;

// 함수 목록
User* create_user(int number, const char* name);
void insert_user(User** head, User* new_user);
void load_users(const char* filename, User** head);
void print_users(User* head);

#endif // USERS_H