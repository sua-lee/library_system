#include <stdlib.h>
#include <stdlib.h>
#include <stdio.h> 
#include "book_tree.h"
#include "genre_hash.h"
#include "author_hash.h"
#include "users.h"
#include "initialization.h"
#include "../common/queue.h" // create_queue() 사용
#include "../common/book_stack.h" // BookStack 관련 헤더

BookStack* g_recent_activity_stack = NULL; 
User* user_list_head = NULL; // main 등에서 접근 가능하도록 전역으로 선언
// book_root는 book_tree.c에 전역으로 선언되어 있음

void initialize_loan_fields_for_user(User* user) {
    if (!user) return;
    user->overdue = 0;
    user->borrowed_num = 0;
    user->reserved_book_node = NULL;
    user->borrowed_book_list_head = NULL;
    user->overdue_book_list_head = NULL;
}

void initialize_loan_fields_for_book(BookNode* book) {
    if (!book) return;
    book->is_available_now = 1;
    book->reservation_queue = create_queue(); // common/queue.c 의 함수 사용
    book->reservation_list_num = 0;
}

void initialize_all_books_loan_fields_recursive(BookNode* root) {
    if (root == NULL) return;
    initialize_loan_fields_for_book(root);
    initialize_all_books_loan_fields_recursive(root->left);
    initialize_all_books_loan_fields_recursive(root->right);
}

void initialization (void) { //
    load_users("users.csv", &user_list_head); //
    User* current_user = user_list_head;
    while (current_user != NULL) {
        initialize_loan_fields_for_user(current_user);
        current_user = current_user->next;
    }
    printf("사용자 정보 및 대출 필드 초기화 완료.\n");

    initialize_book_tree("books.csv"); //
    initialize_all_books_loan_fields_recursive(book_root);
    printf("도서 트리 및 대출 필드 초기화 완료.\n");

    initialize_genre_hash("books.csv"); //
    printf("장르 해시 테이블 초기화 완료.\n");

    initialize_author_hash_from_genre_hash(); //
    printf("저자 해시 테이블 초기화 완료.\n");

    // 베스트셀러용 최근 활동 스택 생성
    g_recent_activity_stack = create_book_stack();
    if (g_recent_activity_stack == NULL) {
        fprintf(stderr, "오류: 최근 활동 스택(g_recent_activity_stack) 생성 실패\n");
        // 필요시 프로그램 종료 또는 오류 처리
    } else {
        printf("최근 활동 스택 초기화 완료.\n");
    }
}