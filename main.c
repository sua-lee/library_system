#include <stdio.h>
#include "datastructure/initialization.c" 


int main(void) {

  initialization();

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