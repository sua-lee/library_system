#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user_management.h"
#include "../init_data/initialization.h" // user_list_head, initialize_loan_fields_for_user 등
#include "../common/list.h" // free_bad_list (대출/연체 목록 해제용)
#include "../common/date_utils.h" // print_date 함수 사용을 위해 추가

// 전역 변수 extern 선언 (initialization.c 에 정의)
extern User* user_list_head; //

void display_user_details(User* user) {
    if (!user) {
        printf("사용자 정보가 없습니다.\n");
        return;
    }
    printf("\n--- 회원 정보 ---\n");
    printf("회원 번호: %d\n", user->number);
    printf("이름: %s\n", user->name);
    printf("도서 클럽 회원: %s\n", user->is_book_club_member ? "예" : "아니오");
    printf("현재 대출 권수: %d\n", user->borrowed_num);
    int display_limit = user->is_book_club_member ? MAX_BORROW_LIMIT_CLUB : MAX_BORROW_LIMIT;
    printf("최대 대출 가능 권수: %d권 (%s)\n", display_limit, user->is_book_club_member ? "도서 클럽 회원" : "일반 회원");
    printf("연체 여부: %s\n", user->overdue ? "예" : "아니오");

    if (user->borrowed_book_list_head) {
        printf("대출 중인 도서:\n");
        BookAndDate* current_loan = user->borrowed_book_list_head;
        while (current_loan) {
            if (current_loan->book_information) {
                printf("  - %s (반납 예정일: ", current_loan->book_information->title);
                print_date(current_loan->due_date); //
                printf(")\n");
            }
            current_loan = current_loan->next;
        }
    } else {
        printf("대출 중인 도서가 없습니다.\n");
    }
    // 필요시 예약 정보, 연체 도서 목록 등 추가 출력
    printf("-------------------\n");
}

int add_user_logic(int user_number, const char* name, int is_club_member) {
    if (find_user_by_number(user_list_head, user_number)) { //
        printf("오류: 이미 사용 중인 회원 번호입니다 (%d).\n", user_number);
        return 0;
    }
    // 이름 중복은 허용할 수 있음 (번호로 구분)

    User* new_user = create_user(user_number, name); //
    if (!new_user) {
        printf("오류: 사용자 노드 생성 실패.\n");
        return 0;
    }
    new_user->is_book_club_member = is_club_member; // 입력받은 도서 클럽 상태 설정
    initialize_loan_fields_for_user(new_user); // 대출 관련 필드 초기화

    insert_user(&user_list_head, new_user); // users.c 함수, 리스트 맨 앞에 삽입
    printf("회원 '%s'(번호: %d) 추가 완료.\n", name, user_number);
    return 1;
}

int delete_user_logic(int user_number) {
    User* user_to_delete = find_user_by_number(user_list_head, user_number); //
    if (!user_to_delete) {
        printf("오류: 회원 번호 %d에 해당하는 사용자를 찾을 수 없습니다.\n", user_number);
        return 0;
    }

    // 삭제 정책: 대출 중인 책이나 예약한 책이 있으면 삭제 불가
    if (user_to_delete->borrowed_num > 0) { //
        printf("오류: 사용자 '%s'님은 현재 대출 중인 도서가 있어 삭제할 수 없습니다.\n", user_to_delete->name);
        return 0;
    }
    if (user_to_delete->reserved_book_node != NULL) { //
        printf("오류: 사용자 '%s'님은 현재 예약 중인 도서가 있어 삭제할 수 없습니다.\n", user_to_delete->name);
        // 필요시 예약 자동 취소 로직 추가 (loan_management.c의 연체 시 예약 취소 로직 참조 가능)
        return 0;
    }
    // 만약 사용자가 어떤 책의 예약 큐에 있다면? (이 부분은 현재 User 구조체에서 직접 알 수 없음)
    //  - 도서 삭제 시 예약 큐를 순회하며 해당 사용자를 제거하는 로직이 필요할 수 있음 (여기서는 생략)


    // 연결 리스트에서 노드 제거
    User* current = user_list_head;
    User* prev = NULL;
    while (current != NULL && current->number != user_number) {
        prev = current;
        current = current->next;
    }

    if (current == NULL) { // 이론상 find_user_by_number 에서 걸러졌어야 함
        printf("오류: 삭제 중 사용자를 다시 찾을 수 없습니다.\n");
        return 0;
    }

    if (prev == NULL) { // 삭제할 노드가 헤드인 경우
        user_list_head = current->next;
    } else {
        prev->next = current->next;
    }

    // 사용자 노드 메모리 해제
    if (user_to_delete->borrowed_book_list_head) {
         free_bad_list(user_to_delete->borrowed_book_list_head); //
         user_to_delete->borrowed_book_list_head = NULL;
    }
    if (user_to_delete->overdue_book_list_head) {
        free_bad_list(user_to_delete->overdue_book_list_head); //
        user_to_delete->overdue_book_list_head = NULL;
    }

    printf("회원 '%s'(번호: %d) 삭제 완료.\n", user_to_delete->name, user_to_delete->number);
    free(user_to_delete);
    return 1;
}

User* search_user_by_number_logic(int user_number) {
    User* user = find_user_by_number(user_list_head, user_number); //
    if (user) {
        display_user_details(user);
    } else {
        printf("회원 번호 %d에 해당하는 사용자를 찾을 수 없습니다.\n", user_number);
    }
    return user;
}

User* search_user_by_name_logic(const char* name) {

    printf("\n--- 회원 이름 '%s' 검색 결과 ---\n", name);
    User* current = user_list_head;
    int found_count = 0;
    User* first_found = NULL; // 간단히 첫 번째 사용자만 반환 (기존 함수와 동일하게)

    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            display_user_details(current);
            if (found_count == 0) {
                first_found = current;
            }
            found_count++;
        }
        current = current->next;
    }

    if (found_count == 0) {
        printf("이름 '%s'에 해당하는 사용자를 찾을 수 없습니다.\n", name);
    } else if (found_count > 1) {
        printf("총 %d명의 동명이인이 검색되었습니다.\n", found_count);
    }
    printf("--------------------------\n");
    return first_found; // 일단 첫 번째 찾은 사용자 반환 (또는 NULL)
}

int update_user_club_status_logic(int user_number, int new_status) {
    User* user = find_user_by_number(user_list_head, user_number); //
    if (!user) {
        printf("오류: 회원 번호 %d에 해당하는 사용자를 찾을 수 없습니다.\n", user_number);
        return 0;
    }
    user->is_book_club_member = (new_status == 1) ? 1 : 0;
    printf("회원 번호 %d 님의 도서 클럽 상태가 '%s'(으)로 변경되었습니다.\n", user_number, user->is_book_club_member ? "가입" : "미가입");
    return 1;
}


// --- UI 함수 구현 ---
void add_new_user_ui() {
    int number, club_status_choice;
    char name[MAX_NAME_LEN];
    User* temp = user_list_head;
    int max_num = 0;
    while(temp){
        if(temp->number > max_num) max_num = temp->number;
        temp = temp->next;
    }
    number = max_num + 1;
    printf("새로운 회원 번호는 %d 입니다.\n", number);

    printf("이름: ");
    scanf(" %[^\n]", name); // 공백 포함 이름 입력 받기
    printf("도서 클럽에 가입하시겠습니까? (1: 예, 0: 아니오): ");
    scanf("%d", &club_status_choice);
    while(getchar() != '\n'); // 입력 버퍼 정리

    add_user_logic(number, name, club_status_choice);
}

void delete_user_ui() {
    int number;
    printf("삭제할 회원 번호: ");
    scanf("%d", &number);
    while(getchar() != '\n');
    delete_user_logic(number);
}

void search_user_ui() {
    int choice, number_input;
    char name_input[MAX_NAME_LEN];
    printf("\n--- 회원 검색 옵션 ---\n");
    printf("1. 번호로 검색\n");
    printf("2. 이름으로 검색\n");
    printf("0. 이전 메뉴로\n");
    printf("선택: ");
    scanf("%d", &choice);
    while(getchar() != '\n');

    switch (choice) {
        case 1:
            printf("검색할 회원 번호: ");
            scanf("%d", &number_input);
            while(getchar() != '\n');
            search_user_by_number_logic(number_input);
            break;
        case 2:
            printf("검색할 회원 이름: ");
            scanf(" %[^\n]", name_input);
            search_user_by_name_logic(name_input);
            break;
        case 0: return;
        default: printf("잘못된 선택입니다.\n");
    }
}

void update_user_info_ui() {
    int number, choice;
    printf("정보를 수정할 회원 번호: ");
    scanf("%d", &number);
    while(getchar() != '\n');

    User* user = find_user_by_number(user_list_head, number); //
    if(!user){
        printf("회원 번호 %d를 찾을 수 없습니다.\n", number);
        return;
    }
    display_user_details(user);
    printf("어떤 정보를 수정하시겠습니까?\n");
    printf("1. 도서 클럽 가입 상태 변경\n");
    printf("0. 취소\n");
    printf("선택: ");
    scanf("%d", &choice);
    while(getchar() != '\n');

    if(choice == 1){
        int new_status_choice;
        printf("새로운 도서 클럽 상태 (1: 가입, 0: 미가입): ");
        scanf("%d", &new_status_choice);
        while(getchar() != '\n');
        update_user_club_status_logic(number, new_status_choice);
    } else if (choice == 0) {
        printf("수정을 취소했습니다.\n");
    } else {
        printf("잘못된 선택입니다.\n");
    }
}


void handle_user_management_menu() {
    int choice;
    while (1) {
        printf("\n--- 회원 관리 메뉴 ---\n");
        printf("1. 회원 추가\n");
        printf("2. 회원 삭제\n");
        printf("3. 회원 검색\n");
        printf("4. 회원 정보 수정 (도서 클럽 상태)\n");
        printf("0. 메인 메뉴로\n");
        printf("선택: ");

        if (scanf("%d", &choice) != 1) {
            printf("잘못된 입력입니다. 숫자를 입력해주세요.\n");
            while(getchar() != '\n'); 
            continue;
        }
        while(getchar() != '\n'); 

        switch (choice) {
            case 1: add_new_user_ui(); break;
            case 2: delete_user_ui(); break;
            case 3: search_user_ui(); break;
            case 4: update_user_info_ui(); break;
            case 0: return;
            default: printf("잘못된 선택입니다.\n");
        }
    }
}