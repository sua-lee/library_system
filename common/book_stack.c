#include "book_stack.h"
#include <stdio.h>
#include <stdlib.h>

BookStack* create_book_stack() {
    BookStack* s = (BookStack*)malloc(sizeof(BookStack));
    if (!s) {
        perror("BookStack 생성 실패");
        exit(EXIT_FAILURE);
    }
    s->top = NULL;
    s->count = 0;
    return s;
}

void push_book_to_stack(BookStack* s, BookNode* book) {
    if (!s || !book) return;
    BookStackNode* newNode = (BookStackNode*)malloc(sizeof(BookStackNode));
    if (!newNode) {
        perror("BookStackNode 생성 실패 (push_book_to_stack)");
        return;
    }
    newNode->book_data = book;
    newNode->next = s->top;
    s->top = newNode;
    s->count++;
}

BookNode* pop_book_from_stack(BookStack* s) {
    if (is_book_stack_empty(s)) return NULL;
    BookStackNode* temp = s->top;
    BookNode* bookData = temp->book_data;
    s->top = s->top->next;
    free(temp);
    s->count--;
    return bookData;
}

BookNode* peek_book_stack_top(BookStack* s) {
    if (is_book_stack_empty(s)) return NULL;
    return s->top->book_data;
}

int is_book_stack_empty(BookStack* s) {
    return (s == NULL || s->top == NULL);
}

int get_book_stack_size(BookStack* s) {
    if (!s) return 0;
    return s->count;
}

void free_book_stack(BookStack* s) {
    if (!s) return;
    while (!is_book_stack_empty(s)) {
        pop_book_from_stack(s); // BookNode 자체는 free하지 않음
    }
    free(s);
}

// 스택을 순회하며 각 노드의 book_data에 대해 주어진 함수를 실행하는 함수
// 스택의 원본을 변경하지 않음 (읽기 전용 순회)
void traverse_book_stack_for_analysis(BookStack* s, void (*process_book_for_analysis)(BookNode* book, void* context), void* context_data) {
    if (is_book_stack_empty(s) || !process_book_for_analysis) return;
    BookStackNode* current = s->top;
    while (current != NULL) {
        process_book_for_analysis(current->book_data, context_data);
        current = current->next;
    }
}