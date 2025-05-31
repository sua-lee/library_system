#ifndef BOOK_STACK_H
#define BOOK_STACK_H

#include "../init_data/book_tree.h" // BookNode 구조체 정의를 위함

// 스택 노드 구조체
typedef struct BookStackNode {
    BookNode* book_data;          // 도서 노드 포인터
    struct BookStackNode* next;
} BookStackNode;

// 스택 구조체
typedef struct BookStack {
    BookStackNode* top;
    int count;                  // 스택에 쌓인 아이템 개수 (선택적)
} BookStack;

// 스택 연산 함수 프로토타입
BookStack* create_book_stack();
void push_book_to_stack(BookStack* s, BookNode* book);
BookNode* pop_book_from_stack(BookStack* s); // 베스트셀러 기능에서는 주로 사용 안 함
BookNode* peek_book_stack_top(BookStack* s);
int is_book_stack_empty(BookStack* s);
int get_book_stack_size(BookStack* s); // 선택적
void free_book_stack(BookStack* s);    // 스택 노드들만 해제, BookNode 자체는 해제 안 함
// 스택 순회 및 각 아이템 처리를 위한 함수 포인터 사용 (디버깅/빈도계산용)
void traverse_book_stack_for_analysis(BookStack* s, void (*process_book_for_analysis)(BookNode* book, void* context), void* context_data);


#endif // BOOK_STACK_H