#ifndef BOOK_TREE_H
#define BOOK_TREE_H

// 도서 노드: 제목 기준 BST
typedef struct BookNode {
  char title[100];
  char author[50];
  char genre[30];
  char sub_genre[50];

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

#endif