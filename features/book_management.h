#ifndef BOOK_MANAGEMENT_H
#define BOOK_MANAGEMENT_H

// 필요한 구조체 정의를 포함하기 위해 BookNode와 User 구조체가 정의된 헤더 파일을 포함
#include "../init_data/book_tree.h" // BookNode 구조체 정의 포함
#include "../init_data/users.h"   // User 구조체 정의 포함 (display_book_details 함수에서 사용)

// --- 사용자 인터페이스(UI) 함수 ---
void handle_book_management_menu();
void add_new_book_ui();
void delete_book_ui();
void search_book_ui();


// --- 핵심 로직 함수 ---

int add_book_logic(const char* title, const char* author, const char* genre, const char* sub_genre);
int delete_book_logic(const char* title);
void search_books_by_title_logic(const char* title);
void search_books_by_author_logic(const char* author_name);
void search_books_by_genre_logic(const char* genre_name, const char* sub_genre_name);


// --- 유틸리티 함수 ---
void display_book_details(BookNode* book, User* current_user_list_head);
void display_book_details(BookNode* book, User* user_list_head_for_due_date_check); // 기존 상세 정보 표시
void display_book_summary_author(BookNode* book); // 간결한 정보 표시 (목록용)
void display_book_summary_genre(BookNode* book);



#endif // BOOK_MANAGEMENT_H
