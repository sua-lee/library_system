#ifndef INITIALIZATION_H
#define INITIALIZATION_H

#include "../common/book_stack.h"
#include "users.h"    // User 구조체 사용을 위해
#include "book_tree.h" // BookNode 구조체 사용을 위해

// 전역 변수 선언 (실제 정의는 initialization.c 에 있음)
extern User* user_list_head;
extern BookStack* g_recent_activity_stack;

// 함수 목록
void initialize_loan_fields_for_user(User* user);
void initialize_loan_fields_for_book(BookNode* book);
void initialize_all_books_loan_fields_recursive(BookNode* root);
void initialization (void);


#endif // INITIALIZATION_H