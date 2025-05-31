#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "init_data/initialization.h" // initialization() 및 전역 변수 사용
#include "features/loan_management.h"
#include "common/structures.h"      // Date 구조체 직접 사용 안하지만, loan_management.h가 사용
#include "common/date_utils.h"      // get_current_date() 등

// main.c는 이전 답변과 거의 동일하게 유지될 수 있습니다.
// find_user_by_number 와 find_book_by_title 함수는
// 각각 init-data/users.c 와 init-data/book_tree.c 에 구현되어야 합니다.

// 함수 프로토타입
void display_main_menu();
void handle_loan_menu_options(); // 이름 변경 (handle_loan_menu -> handle_loan_menu_options)

// main 함수 (이전 답변의 main.c 내용 참조)
int main(void) {
    initialization();

    int main_sel;
    while (1) {
        display_main_menu();
        printf("선택: ");
        if (scanf("%d", &main_sel) != 1) {
            printf("잘못된 입력입니다. 숫자를 입력해주세요.\n");
            while(getchar() != '\n'); 
            continue;
        }
        while(getchar() != '\n'); 

        switch (main_sel) {
            case 1:
                printf("도서 관리 기능은 아직 구현되지 않았습니다.\n"); //
                break;
            case 2:
                printf("회원 관리 기능은 아직 구현되지 않았습니다.\n"); //
                // print_users(user_list_head); // 디버깅용 (user_list_head는 initialization.c에 정의된 전역변수)
                break;
            case 3:
                handle_loan_menu_options();
                break;
            case 4:
                printf("베스트셀러 기능은 아직 구현되지 않았습니다.\n"); //
                break;
            case 0:
                printf("프로그램을 종료합니다.\n");
                // TODO: 메모리 해제
                exit(0);
            default:
                printf("잘못된 입력입니다.\n"); //
        }
        printf("\n");
    }
    return 0;
}

void display_main_menu() {
    printf("=======================\n");
    printf("도서관 관리 시스템\n");
    printf("=======================\n");
    printf("1. 도서 관리 (미구현)\n");
    printf("2. 회원 관리 (미구현)\n");
    printf("3. 대출/반납 관리\n");
    printf("4. 베스트셀러 (미구현)\n");
    printf("0. 종료\n");
    printf("=======================\n");
}

void handle_loan_menu_options() {
    int sel;
    printf("\n--- 대출/반납 관리 ---\n");
    printf("1. 도서 대출 (및 예약)\n");
    printf("2. 도서 반납\n");
    printf("0. 메인 메뉴로 돌아가기\n");
    printf("선택: ");
    if (scanf("%d", &sel) != 1) {
        printf("잘못된 입력입니다.\n");
        while(getchar() != '\n');
        return;
    }
    while(getchar() != '\n');

    if (sel == 0) return;

    // user_list_head 와 book_root 는 initialization.c 에 정의된 전역변수 사용
    extern User* user_list_head; 
    extern BookNode* book_root; 

    if (sel == 1) { 
        int user_number;
        char book_title[100];
        printf("대출할 회원 번호: ");
        if (scanf("%d", &user_number) != 1) { 
          printf("잘못된 입력입니다.\n"); 
          while(getchar() != '\n'){
            ;
          } return; 
        }
        while(getchar() != '\n');

        printf("대출할 도서 제목: ");
        if (fgets(book_title, sizeof(book_title), stdin) == NULL) { printf("입력 오류.\n"); return;}
        book_title[strcspn(book_title, "\n")] = 0; 

        User* user = find_user_by_number(user_list_head, user_number); 
        BookNode* book = find_book_by_title(book_root, book_title);   

        if (!user) {
            printf("오류: 회원번호 %d를 찾을 수 없습니다.\n", user_number);
            return;
        }
        if (!book) {
            printf("오류: 도서 '%s'를 찾을 수 없습니다.\n", book_title);
            return;
        }
        Date current_d = get_current_date();
        handle_loan_request(user, book, current_d);

    } else if (sel == 2) { 
        int user_number;
        char book_title[100];
        printf("반납할 회원 번호: ");
        if (scanf("%d", &user_number) != 1) { 
          printf("잘못된 입력입니다.\n"); 
          while(getchar() != '\n') {
            ;
          }
          return; }
        while(getchar() != '\n');

        printf("반납할 도서 제목: ");
        if (fgets(book_title, sizeof(book_title), stdin) == NULL) { printf("입력 오류.\n"); return;}
        book_title[strcspn(book_title, "\n")] = 0;

        User* user = find_user_by_number(user_list_head, user_number);
        BookNode* book = find_book_by_title(book_root, book_title);

        if (!user) {
            printf("오류: 회원번호 %d를 찾을 수 없습니다.\n", user_number);
            return;
        }
        if (!book) {
            printf("오류: 도서 '%s'를 찾을 수 없습니다.\n", book_title);
            return;
        }
        Date current_d = get_current_date();
        handle_return_request(user, book, current_d);
    } else {
        printf("잘못된 선택입니다.\n");
    }
}