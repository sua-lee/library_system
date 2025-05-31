#ifndef BOOK_TREE_H
#define BOOK_TREE_H


#include "../common/structures.h" // BookAndDate 사용을 위해
struct Queue; // common/queue.h 에 정의된 Queue

// 도서 노드: 제목 기준 BST
typedef struct BookNode {
    char title[100];
    char author[50];
    char genre[30];
    char sub_genre[50];
  
    int is_available_now;
    struct Queue* reservation_queue;
    int reservation_list_num;

    int cumulative_loan_count;
    struct BookAndDate* loan_history_head;

    struct BookNode *left;
    struct BookNode *right;

} BookNode;

extern BookNode* book_root;

BookNode* create_book_node(const char* title, const char* author, const char* genre, const char* sub_genre);
void insert_book_node(BookNode** root, BookNode* new_node);
void initialize_book_tree(const char* filename);
void parse_line_with_quotes(char* line, char** title, char** author, char** genre, char** sub_genre);
void parse_line_without_quotes(char* line, char** title, char** author, char** genre, char** sub_genre);
void print_book_tree(BookNode* root);
char* strip_quotes(char* str);
void create_and_insert_tree(const char* title, const char* author, const char* genre, const char* sub_genre);
BookNode* find_book_by_title(BookNode* root, const char* title);
void fully_free_book_node(BookNode* node); // BookNode와 내부 동적 메모리 모두 해제
BookNode* delete_from_bst_by_title_recursive(BookNode** node_ref, const char* title_to_delete); // BST 삭제 (이름 변경 및 역할 명확화)
BookNode* find_min_val_bst_node(BookNode* node);

#endif