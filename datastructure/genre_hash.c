#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "genre_hash.h"
#include "book_tree.h"  // sub_genre별 book BST 생성을 위해

// 해시 테이블 초기화
GenreBucket* genre_hash_table[HASH_TABLE_SIZE] = { NULL };

int hash_string(const char* str, int table_size) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }

    return (int)(hash % table_size);
}

// 새로운 GenreBucket을 동적으로 생성하여 초기화.
// 장르 이름 (예: "fiction", "science")
// return 초기화된 GenreBucket 포인터
GenreBucket* create_genre_bucket(const char* genre) {
    GenreBucket* bucket = (GenreBucket*)malloc(sizeof(GenreBucket));
    if (bucket == NULL) {
        fprintf(stderr, "메모리 할당 실패: GenreBucket\n");
        exit(1);
    }

    strcpy(bucket->genre, genre);
    bucket->sub_genre_list = NULL;
    bucket->next = NULL;

    return bucket;
}

// 새로운 SubGenreNode를 동적으로 생성하여 초기화합니다.
// sub_genre 서브장르 이름 (예: "novel", "classic")
// return 초기화된 SubGenreNode 포인터
SubGenreNode* create_sub_genre_node(const char* sub_genre) {
    SubGenreNode* node = (SubGenreNode*)malloc(sizeof(SubGenreNode));
    if (node == NULL) {
        fprintf(stderr, "메모리 할당 실패: SubGenreNode\n");
        exit(1);
    }

    strcpy(node->sub_genre, sub_genre);
    node->book_bst = NULL;
    node->next = NULL;

    return node;
}

// 진입점 함수
// 파싱함수는 book_tree.h에서 정의한 함수 사용.
void initialize_genre_hash(const char* filename) {
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
      insert_into_genre_hash_table(title, author, genre, sub_genre);
    }
  }

  fclose(file);
}

void insert_into_genre_hash_table(const char* title, const char* author, const char* genre, const char* sub_genre) {
  // 1. BookNode 생성
  BookNode* book = create_book_node(title, author, genre, sub_genre);
  if (!book) {
    fprintf(stderr, "BookNode 생성 실패\n");
    return;
  }

  // 2. 해시 인덱스 계산
  int index = hash_string(genre, HASH_TABLE_SIZE);
  GenreBucket* current = genre_hash_table[index];

  // 3. 체이닝을 따라 동일한 genre 탐색
  while (current != NULL) {
    if (strcmp(current->genre, genre) == 0) break;
    current = current->next;
  }

  // 4. 기존 genre가 없다면 새로운 버킷 생성
  if (!current) {
    current = create_genre_bucket(genre);
    current->next = genre_hash_table[index];
    genre_hash_table[index] = current;
  }

  // 5. 해당 버킷에 sub_genre 삽입 (존재 시 해당 노드 반환)
  // 6. 해당 sub_genre의 BST에 Book 삽입
  SubGenreNode* sub = insert_sub_genre_list(&(current->sub_genre_list), sub_genre);
  insert_book_node(&(sub->book_bst), book);
}

SubGenreNode* insert_sub_genre_list(SubGenreNode** head, const char* sub_genre) {
  SubGenreNode* current = *head;

  // 1. 이미 존재하는 서브장르 노드가 있는지 확인
  while (current != NULL) {
    if (strcmp(current->sub_genre, sub_genre) == 0) {
      return current;  // 이미 존재하는 노드를 반환
    }
    current = current->next;
  }

  // 2. 없으면 create_sub_genre_node() 호출하여 리스트 앞에 삽입
  SubGenreNode* new_node = create_sub_genre_node(sub_genre);
  new_node->next = *head;
  *head = new_node;

  return new_node;
}

// 디버깅용 출력함수 debug_print_book_bst, void debug_print_genre_hash_table()
void debug_print_book_bst(BookNode* root, const char* prefix, int is_last) {
    if (!root) return;

    // 현재 노드 출력
    printf("%s%s 제목: %s | 저자: %s\n", prefix, is_last ? "└─" : "├─", root->title, root->author);

    // 새로운 prefix 설정
    char new_prefix[256];
    snprintf(new_prefix, sizeof(new_prefix), "%s%s", prefix, is_last ? "   " : "│  ");

    // 재귀적으로 좌우 노드 출력
    if (root->left || root->right) {
        if (root->left)
            debug_print_book_bst(root->left, new_prefix, root->right == NULL);
        if (root->right)
            debug_print_book_bst(root->right, new_prefix, 1);
    }
}

void debug_print_genre_hash_table(void) {
    printf("장르 해시 테이블 구조 출력\n\n");

    for (int i = 0; i < HASH_TABLE_SIZE; ++i) {
        GenreBucket* genre = genre_hash_table[i];
        if (!genre) continue;

        printf("해시 인덱스 %d:\n", i);

        while (genre) {
            printf("└─ 장르: %s\n", genre->genre);

            SubGenreNode* sub = genre->sub_genre_list;
            while (sub) {
                printf("   ├─ 서브장르: %s\n", sub->sub_genre);
                if (sub->book_bst)
                    debug_print_book_bst(sub->book_bst, "   │  ", 1);
                else
                    printf("   │  └─ (도서 없음)\n");
                sub = sub->next;
            }

            genre = genre->next;
        }

        printf("\n");
    }
}