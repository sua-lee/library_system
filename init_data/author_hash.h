#ifndef AUTHOR_HASH_H
#define AUTHOR_HASH_H

#include "book_tree.h"  // BookNode 구조체 포함

#define AUTHOR_HASH_TABLE_SIZE 101  // 해시 테이블 크기 (소수로 충돌 줄임)

// 도서 리스트 노드 (연결 리스트로 구성)
typedef struct BookListNode {
    BookNode* book;
    struct BookListNode* next;
} BookListNode;

// 작가 해시 테이블의 버킷 구조
typedef struct AuthorBucket {
    char author[50];                // 작가 이름 (최대 49자 + '\0')
    BookListNode* book_list_head;  // 해당 작가의 도서 리스트
    struct AuthorBucket* next;     // 해시 충돌 시 체이닝
} AuthorBucket;

// 전역 해시 테이블
extern AuthorBucket* author_hash_table[AUTHOR_HASH_TABLE_SIZE];

// 함수 선언
void insert_into_author_hash_table(BookNode* book);
void initialize_author_hash_from_genre_hash(void);
void insert_books_from_bst(BookNode* root);
void debug_print_author_hash_table(void);
int remove_book_from_author_hash(BookNode* book_to_delete);



#endif  // AUTHOR_HASH_H