#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "book_tree.h"

BookNode* book_root = NULL;

// BookNode 생성 함수
BookNode* create_book_node(const char* title, const char* author, const char* genre, const char* sub_genre) {
  BookNode* node = (BookNode*)malloc(sizeof(BookNode));
  if (!node) {
    fprintf(stderr, "메모리 할당 실패\n");
      exit(1);
  }

  strcpy(node->title, title);
  strcpy(node->author, author);
  strcpy(node->genre, genre);
  strcpy(node->sub_genre, sub_genre);
  node->left = node->right = NULL;

  return node;
}

// 새 도서 노드를 제목 기준 BST에 삽입하는 함수
// - 루트 포인터를 받아서 트리에 재귀적으로 노드를 삽입함
// - strcmp를 사용하여 알파벳 순으로 왼쪽/오른쪽 서브트리에 배치함 (중위 순회용)
void insert_book_node(BookNode** root, BookNode* new_node) {
    if (*root == NULL) {
        *root = new_node;
        return;
    }

    int cmp = strcmp(new_node->title, (*root)->title);
    if (cmp < 0) {
        insert_book_node(&(*root)->left, new_node);
    } else {
        insert_book_node(&(*root)->right, new_node);
    }
}

// 따옴표 삭제
// BookNode 생성 직전 호출
char* strip_quotes(char* str) {
    if (!str) return NULL;
    int len = strlen(str);
    if (len >= 2 && str[0] == '"' && str[len - 1] == '"') {
        str[len - 1] = '\0'; // 끝 " 제거
        return str + 1;      // 시작 " 이후 포인터 반환
    }
    return str;
}

// 헤더를 제외한 csv 파일의 행을 읽는다.
// 따옴표가 포함된 경우와, 아닌 경우에 대해 서로 다른 파싱 함수를 호출함.
void initialize_book_tree(const char* filename) {
  FILE* file = fopen(filename, "r");
  if (!file) {
    perror("파일 열기 실패");
    return;
  }

  char line[512];
  fgets(line, sizeof(line), file); // 헤더 스킵

  while (fgets(line, sizeof(line), file)) {
    line[strcspn(line, "\r\n")] = '\0';
    char *title = NULL, *author = NULL, *genre = NULL, *sub_genre = NULL;

    if (strchr(line, '"')) {
      parse_line_with_quotes(line, &title, &author, &genre, &sub_genre);  
    } else {
      parse_line_without_quotes(line, &title, &author, &genre, &sub_genre);  
    }

    if (title && author && genre && sub_genre) {
      create_and_insert_tree(title, author, genre, sub_genre);
    }

  }

  fclose(file);
}

// 따옴표 포함 라인 처리
// 라인의 각 토큰을 포인터가 가리키도록 포인터 배열 선언
void parse_line_with_quotes(char* line, char** title, char** author, char** genre, char** sub_genre) {
  char* fields[5] = { NULL };
  int field_index = 0;
  int in_quotes = 0;
  char* field_start = line;

  for (char* ptr = line; *ptr != '\0' && field_index < 5; ++ptr) {
    if (*ptr == '"') {
      in_quotes = !in_quotes;
    } else if (*ptr == ',' && !in_quotes) {
      *ptr = '\0';
      fields[field_index++] = field_start;
      field_start = ptr + 1;
    }
  }

  if (field_index < 5 && *field_start != '\0') {
    fields[field_index++] = field_start;
  }

  if (field_index == 5) {  
    *title = strip_quotes(fields[1]);
    *author = strip_quotes(strlen(fields[2]) > 0 ? fields[2] : "Anonymous");
    *genre = strip_quotes(fields[3]);
    *sub_genre = strip_quotes(fields[4]);
  }
}

// 따옴표 포함하지 않는 라인 처리
void parse_line_without_quotes(char* line, char** title, char** author, char** genre, char** sub_genre) {
  char* fields[5];
  int i = 0;
  char* token = strtok(line, ",");

  while (token && i < 5) {
    fields[i++] = token;
    token = strtok(NULL, ",");
  }

  if (i == 5) {
    *title = fields[1];
    *author = (strlen(fields[2]) > 0) ? fields[2] : "Anonymous";
    *genre = fields[3];
    *sub_genre = fields[4];
  }
}

void create_and_insert_tree(const char* title, const char* author, const char* genre, const char* sub_genre) {
  BookNode* new_book = create_book_node(title, author, genre, sub_genre);
  insert_book_node(&book_root, new_book);
}

// 책 제목을 중위순회로 출력 (디버깅용 함수)
void print_book_tree(BookNode* root) {
    if (root == NULL) return;
    print_book_tree(root->left);
    printf("제목: %s, 저자: %s, 장르: %s, 서브장르: %s\n", root->title, root->author, root->genre, root->sub_genre);
    print_book_tree(root->right);
}

BookNode* find_book_by_title(BookNode* root, const char* title) {
    if (root == NULL) {
        return NULL; // 트리가 비었거나, 해당 경로에 책이 없음
    }

    int cmp = strcmp(title, root->title);

    if (cmp == 0) {
        return root; // 책을 찾음
    } else if (cmp < 0) {
        return find_book_by_title(root->left, title); // 왼쪽 서브트리 검색
    } else {
        return find_book_by_title(root->right, title); // 오른쪽 서브트리 검색
    }
}