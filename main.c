#include <stdio.h>
#include "datastructure/book_tree.h"
#include "datastructure/author_hash.h"
#include "datastructure/genre_hash.h"


int main(void) {
// 1. books.csv 파일로부터 BST 트리 구성
  initialize_book_tree("books.csv");
  initialize_genre_hash("books.csv");
  initialize_author_hash_from_genre_hash();


  // 3. 해시 테이블 출력 (디버깅용)
  debug_print_author_hash_table();


  int main_sel;
  printf("1. 도서 관리\n2. 회원 관리\n3. 대출 관리\n4. 베스트셀러\n선택: ");
  scanf("%d", &main_sel);
  
  switch (main_sel) {
    case 1: printf("아직 기능이 구현되지 않았습니다.\n"); break;
    case 2: printf("아직 기능이 구현되지 않았습니다.\n"); break;
    case 3: printf("아직 기능이 구현되지 않았습니다.\n"); break;
    case 4: printf("아직 기능이 구현되지 않았습니다.\n"); break;
    default: printf("잘못된 입력입니다.\n");
  }
  
  return 0;
}