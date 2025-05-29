// datastructure/book_tree.h

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

// 장르 노드: 장르 + 서브장르 + 장르별 도서 BST
typedef struct GenreNode {
    char genre[30];
    struct GenreNode* left;
    struct GenreNode* right;

    struct GenreNode* sub_genre_bst;    // 서브장르로 BST 구성
    BookNode* book_bst;                 // 이 장르에 속한 책들 (제목 기준 BST)
} GenreNode;

// 저자 노드: 저자 기준 BST + 각 저자의 도서 BST + 장르 BST
typedef struct AuthorNode {
    char author[50];
    struct AuthorNode* left;
    struct AuthorNode* right;

    BookNode* book_bst;    // 이 저자의 책들 (제목 기준 BST)
    GenreNode* genre_bst;    // 이 저자가 다룬 장르 (장르명 기준 BST)
} AuthorNode;

extern BookNode* book_root;
extern GenreNode* genre_root;
extern AuthorNode* author_root;

BookNode* create_book_node(const char* title, const char* author, const char* genre, const char* sub_genre);
void insert_book_node(BookNode** root, BookNode* new_node);
void initialize_book_tree(const char* filename);
void parse_line_with_quotes(char* line);
void parse_line_without_quotes(char* line);
void print_book_tree(BookNode* root);


#endif