#ifndef GENRE_HASH_H
#define GENRE_HASH_H

#include "book_tree.h"  // BookNode 정의를 사용하기 위함

#define HASH_TABLE_SIZE 100

// 서브장르 노드: 하나의 서브장르에 대한 도서 BST
typedef struct SubGenreNode {
    char sub_genre[50];
    BookNode* book_bst;  // 해당 서브장르의 도서 BST
    struct SubGenreNode* next;  // 체이닝
} SubGenreNode;

// 장르 버킷: 하나의 장르에 대해 서브장르들을 연결 리스트로 보유
typedef struct GenreBucket {
    char genre[30];  // 장르명
    SubGenreNode* sub_genre_list;  // 서브장르 연결 리스트
    struct GenreBucket* next;  // 해시 충돌 시 체이닝
} GenreBucket;

// 해시 테이블 선언
extern GenreBucket* genre_hash_table[HASH_TABLE_SIZE];

// 함수 선언
int hash_genre(const char* genre);
GenreBucket* create_genre_bucket(const char* genre);
SubGenreNode* create_sub_genre_node(const char* sub_genre);
void initialize_genre_hash(const char* filename);
void insert_into_genre_hash_table(const char* title, const char* author, const char* genre, const char* sub_genre);
SubGenreNode* insert_sub_genre_list(SubGenreNode** head, const char* sub_genre);
void debug_print_book_bst(BookNode* root, const char* prefix, int is_last);
void debug_print_genre_hash_table(void);

#endif