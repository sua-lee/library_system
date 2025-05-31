#ifndef INITIALIZATION_H
#define INITIALIZATION_H

#include "users.h"    // User 구조체 사용을 위해
#include "book_tree.h" // BookNode 구조체 사용을 위해

// 전역 변수 선언 (실제 정의는 initialization.c 에 있음)
extern User* user_list_head;
// extern BookNode* book_root; // book_root는 book_tree.c에 정의되어 있고, book_tree.h에 extern 선언이 이미 있을 것입니다.

// 함수 프로토타입 선언
void initialization(void);
// 만약 main.c에서 다른 initialization.c 내부 함수를 직접 호출해야 한다면 여기에 추가 선언
// (보통은 initialization() 하나만 호출합니다)

#endif // INITIALIZATION_H