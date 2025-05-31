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

// author_hash.c 에 추가

// 저자 해시 테이블에서 특정 BookNode를 삭제하는 함수
// 주의: 이 함수는 BookNode 자체를 free하지 않습니다. 단지 해시 테이블의 연결 리스트에서만 제거합니다.
int remove_book_from_author_hash(BookNode* book_to_delete) {
    if (!book_to_delete || strlen(book_to_delete->author) == 0) return 0;

    int index = hash_string(book_to_delete->author, AUTHOR_HASH_TABLE_SIZE); //
    AuthorBucket* current_bucket = author_hash_table[index]; //
    AuthorBucket* prev_bucket = NULL;

    while (current_bucket != NULL) {
        if (strcmp(current_bucket->author, book_to_delete->author) == 0) { //
            BookListNode* current_book_node = current_bucket->book_list_head; //
            BookListNode* prev_book_node = NULL;
            while (current_book_node != NULL) {
                if (current_book_node->book == book_to_delete) { // 포인터 주소 비교
                    if (prev_book_node == NULL) {
                        current_bucket->book_list_head = current_book_node->next;
                    } else {
                        prev_book_node->next = current_book_node->next;
                    }
                    free(current_book_node); // BookListNode 자체는 여기서 해제

                    // 만약 AuthorBucket의 book_list_head가 비었다면 AuthorBucket도 삭제 (선택적)
                    if (current_bucket->book_list_head == NULL) {
                        if (prev_bucket == NULL) {
                            author_hash_table[index] = current_bucket->next;
                        } else {
                            prev_bucket->next = current_bucket->next;
                        }
                        free(current_bucket); // AuthorBucket 자체 해제
                    }
                    return 1; // 삭제 성공
                }
                prev_book_node = current_book_node;
                current_book_node = current_book_node->next;
            }
            return 0; // 해당 저자의 책 목록에 없음
        }
        prev_bucket = current_bucket;
        current_bucket = current_bucket->next;
    }
    return 0; // 해당 저자 없음
}