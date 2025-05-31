#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "author_hash.h"
#include "genre_hash.h"  // hash_string 재사용
#include "book_tree.h"   // BookNode 사용

// 전역 해시 테이블 초기화
AuthorBucket* author_hash_table[AUTHOR_HASH_TABLE_SIZE] = { NULL };

// 작가별 해시 인덱스에 BookNode 삽입
void insert_into_author_hash_table(BookNode* book) {
  if (!book) {
    printf("작가명이 없는 도서는 삽입불가. author_hash_table");
    return;
  }

  int index = hash_string(book->author, AUTHOR_HASH_TABLE_SIZE);
  AuthorBucket* current = author_hash_table[index];

  // 동일 작가가 이미 존재하는지 확인
  while (current) {
    if (strcmp(current->author, book->author) == 0) break;
    current = current->next;
  }

  // 작가 버킷이 없다면 새로 생성하여 체이닝
  if (!current) {
    current = (AuthorBucket*)malloc(sizeof(AuthorBucket));
    if (!current) {
      fprintf(stderr, "메모리 할당 실패: AuthorBucket\n");
      exit(1);
    }

    strcpy(current->author, book->author);
    current->book_list_head = NULL;
    current->next = author_hash_table[index];  // 앞에 삽입
    author_hash_table[index] = current;
  }

  // 도서 노드를 작가의 리스트에 앞쪽 삽입
  BookListNode* new_node = (BookListNode*)malloc(sizeof(BookListNode));
  if (!new_node) {
    fprintf(stderr, "메모리 할당 실패: BookListNode\n");
    exit(1);
  }

  new_node->book = book;
  new_node->next = current->book_list_head;
  current->book_list_head = new_node;
}

// genre_hash 테이블을 순회하면서 작가 해시 테이블 초기화
void initialize_author_hash_from_genre_hash(void) {
  for (int i = 0; i < HASH_TABLE_SIZE; ++i) {
    GenreBucket* genre = genre_hash_table[i];
    while (genre) {
      SubGenreNode* sub = genre->sub_genre_list;
      while (sub) {
        insert_books_from_bst(sub->book_bst);          
        sub = sub->next;
      }
      genre = genre->next;
    }
  }
}

// 내부 재귀 함수: BST 순회하며 insert
void insert_books_from_bst(BookNode* root) {
    if (!root) return;

    insert_books_from_bst(root->left);
    insert_into_author_hash_table(root); // 핵심
    insert_books_from_bst(root->right);
}

// 디버깅용 출력 함수
void debug_print_author_hash_table(void) {
    printf("작가 해시 테이블 구조 출력\n\n");

    for (int i = 0; i < AUTHOR_HASH_TABLE_SIZE; ++i) {
        AuthorBucket* author = author_hash_table[i];
        if (!author) continue;

        printf("해시 인덱스 %d:\n", i);

        while (author) {
            printf("└─ 작가: %s\n", author->author);

            BookListNode* node = author->book_list_head;
            while (node) {
                printf("   ├─ 제목: %s | 장르: %s | 서브장르: %s\n",
                       node->book->title,
                       node->book->genre,
                       node->book->sub_genre);
                node = node->next;
            }

            author = author->next;
        }

        printf("\n");
    }
}