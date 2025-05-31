#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "book_management.h"
#include "../init_data/initialization.h" // user_list_head, book_root 등 전역 변수 접근
#include "../init_data/genre_hash.h"   // add_existing_book_to_genre_hash, remove_book_from_genre_hash 등
#include "../init_data/author_hash.h"  // insert_into_author_hash_table, remove_book_from_author_hash 등
#include "../common/date_utils.h"      // print_date 등
#include "../common/list.h"            // BookAndDate 리스트용
#include "../common/queue.h"           // peek_front_user, is_queue_empty


// init_data/initialization.c 또는 해당 .h 파일에 선언되어 있어야 함
extern BookNode* book_root; //
extern User* user_list_head; //
extern AuthorBucket* author_hash_table[AUTHOR_HASH_TABLE_SIZE]; // author_hash.c 에 정의
extern GenreBucket* genre_hash_table[HASH_TABLE_SIZE];      // genre_hash.c 에 정의


// genre_hash.c 와 author_hash.c 에 추가된 함수들의 프로토타입이 해당 .h 파일에 있어야 함
// 또는 여기에 extern 선언 (권장하지 않음)
// void add_existing_book_to_genre_hash(BookNode* book_to_add); // genre_hash.h 에 선언 가정
// int remove_book_from_genre_hash(BookNode* book_to_delete);  // genre_hash.h 에 선언 가정
// int remove_book_from_author_hash(BookNode* book_to_delete); // author_hash.h 에 선언 가정

void display_book_summary_author(BookNode* book) {
    if (!book) return;
    // 예시 출력: "제목: The Great Gatsby | 저자: F. Scott Fitzgerald"
    // 또는 "제목: The Great Gatsby | 장르: fiction | 서브장르: classic"
    // 여기서는 제목과 저자만 간결하게 출력하겠습니다.
    printf("  └─ 제목: %s | 장르: %s | 서브 장르: %s\n", book->title, book->genre, book->sub_genre);
}

void display_book_summary_genre(BookNode* book) {
    if (!book) return;
    // 예시 출력: "제목: The Great Gatsby | 저자: F. Scott Fitzgerald"
    // 또는 "제목: The Great Gatsby | 장르: fiction | 서브장르: classic"
    // 여기서는 제목과 저자만 간결하게 출력하겠습니다.
    printf("  │  └─ 제목: %s | 저자: %s\n", book->title, book->author);
}

static void display_books_in_genre_bst_summary(BookNode* bst_root) {
    if (bst_root == NULL) return;
    // 중위 순회 (왼쪽 -> 현재 -> 오른쪽) 또는 원하는 순서로 변경 가능
    display_books_in_genre_bst_summary(bst_root->left);
    // *** 수정된 부분: display_book_details 대신 display_book_summary 호출 ***
    display_book_summary_genre(bst_root);
    display_books_in_genre_bst_summary(bst_root->right);
}

int add_book_logic(const char* title, const char* author, const char* genre, const char* sub_genre) {
    if (find_book_by_title(book_root, title)) { //
        printf("오류: 동일한 제목의 도서('%s')가 이미 존재합니다.\n", title);
        return 0;
    }

    BookNode* new_book = create_book_node(title, author, genre, sub_genre); //
    if (!new_book) {
        printf("오류: 도서 노드 생성 실패.\n");
        return 0;
    }
    initialize_loan_fields_for_book(new_book); // 대출 관련 필드 초기화 (큐 생성 등)

    // 1. 메인 BST에 삽입
    insert_book_node(&book_root, new_book); //

    // 2. 장르 해시 테이블에 삽입 (주의: new_book 포인터 공유)
    add_existing_book_to_genre_hash(new_book); // genre_hash.c에 구현된 함수 호출

    // 3. 저자 해시 테이블에 삽입 (주의: new_book 포인터 공유)
    insert_into_author_hash_table(new_book); //

    printf("도서 '%s' 추가 완료.\n", title);
    return 1;
}

int delete_book_logic(const char* title) {
    BookNode* book_to_delete_main_bst_ref; // 메인 BST에서 찾은 실제 노드 포인터

    book_to_delete_main_bst_ref = find_book_by_title(book_root, title); //
    if (!book_to_delete_main_bst_ref) {
        printf("오류: 제목 '%s'에 해당하는 도서를 메인 트리에서 찾을 수 없습니다.\n", title);
        return 0;
    }

    // 삭제 정책 확인
    if (book_to_delete_main_bst_ref->is_available_now == 0 || book_to_delete_main_bst_ref->is_available_now == 2) { //
        printf("오류: 도서 '%s'는 현재 대출 중이거나 수령 대기 중이므로 삭제할 수 없습니다.\n", title);
        return 0;
    }
    if (book_to_delete_main_bst_ref->reservation_list_num > 0) { //
        printf("오류: 도서 '%s'에 예약자가 있어 삭제할 수 없습니다.\n", title);
        return 0;
    }

    // 1. 장르 해시 테이블에서 삭제 (BookNode 포인터 기반으로 내부 BST에서 제거)
    if (!remove_book_from_genre_hash(book_to_delete_main_bst_ref)) {
        // 여기서 실패하면 데이터 불일치가 발생할 수 있음. 로그 남기거나 예외처리.
        printf("경고: 장르 해시 테이블에서 '%s' 삭제 실패 또는 찾지 못함.\n", title);
    }

    // 2. 저자 해시 테이블에서 삭제 (BookNode 포인터 기반으로 내부 리스트에서 제거)
    if (!remove_book_from_author_hash(book_to_delete_main_bst_ref)) {
        printf("경고: 저자 해시 테이블에서 '%s' 삭제 실패 또는 찾지 못함.\n", title);
    }
    
    // 3. 메인 BST에서 삭제
    // book_root를 직접 수정하며 삭제.
    BookNode* node_actually_deleted_from_main_bst = delete_from_bst_by_title_recursive(&book_root, title);

    if (node_actually_deleted_from_main_bst) { 
        // 4. BookNode 메모리 및 내부 할당 메모리 최종 해제
        // 이 BookNode는 모든 자료구조에서 참조가 제거되었으므로 안전하게 해제 가능.
        fully_free_book_node(book_to_delete_main_bst_ref); // book_to_delete_main_bst_ref 가 실제 삭제된 원본 노드
        printf("도서 '%s' 삭제 완료.\n", title);
        return 1;
    } else {
        printf("오류: 메인 BST에서 도서 '%s' 삭제 중 문제 발생.\n", title);
        return 0;
    }
}

void display_book_details(BookNode* book, User* current_user_list_head) {
    if (!book) return;

    printf("\n--- 도서 정보 ---\n");
    printf("제목: %s\n", book->title);
    printf("저자: %s\n", book->author);
    printf("장르: %s (서브: %s)\n", book->genre, book->sub_genre);
    printf("누적 대출 횟수: %d\n", book->cumulative_loan_count);

    printf("현재 상태: ");
    if (book->is_available_now == 1) { //
        printf("대출 가능\n");
    } else if (book->is_available_now == 0) { //
        printf("대출 중");
        User* u = current_user_list_head;
        int found_loan = 0;
        while (u) {
            BookAndDate* bad = u->borrowed_book_list_head; //
            while (bad) {
                if (bad->book_information == book) { // 포인터 비교
                    printf(" (대출자: %s, 반납 예정일: ", u->name); //
                    print_date(bad->due_date); //
                    printf(")\n");
                    found_loan = 1;
                    break;
                }
                bad = bad->next;
            }
            if (found_loan) break;
            u = u->next;
        }
        if (!found_loan) printf(" (대출 정보 상세 확인 불가 - 현재 대출자 정보 직접 조회 필요)\n");

    } else if (book->is_available_now == 2) { //
        printf("예약자 수령 대기 중\n");
    }

    printf("예약자 수: %d\n", book->reservation_list_num); //
    if (book->reservation_list_num > 0 && book->reservation_queue && !is_queue_empty(book->reservation_queue)) { //
        printf("  ㄴ 다음 예약자: %s\n", peek_front_user(book->reservation_queue)->name); //
    }
    
    printf("대출 이력:\n");
    if (book->loan_history_head == NULL) {
        printf("  (이력이 없습니다.)\n");
    } else {
        BookAndDate* current_hist = book->loan_history_head;
        int count = 1;
        while(current_hist) {
            printf("  %d. ", count++);
            if(current_hist->user_information) printf("대출자: %s, ", current_hist->user_information->name);
            else printf("대출자 정보 없음, ");
            printf("대출일: "); print_date(current_hist->action_date); //
            printf(", 반납(완료)일: "); print_date(current_hist->due_date); //
            printf("\n");
            current_hist = current_hist->next;
        }
    }
    printf("-------------------\n");
}

void search_books_by_title_logic(const char* title) {
    BookNode* book = find_book_by_title(book_root, title); 
    if (book) {
        display_book_details(book, user_list_head);
    } else {
        printf("제목 '%s'에 해당하는 도서를 찾을 수 없습니다.\n", title);
    }
}

void search_books_by_author_logic(const char* author_name) {
    // display_book_details 호출 시 user_list_head 전달
    int index = hash_string(author_name, AUTHOR_HASH_TABLE_SIZE); //
    AuthorBucket* author_bucket = author_hash_table[index]; //
    int found_count = 0;

    printf("\n--- 저자 '%s' 검색 결과 ---\n", author_name);
    while (author_bucket) {
        if (strcmp(author_bucket->author, author_name) == 0) { //
            BookListNode* book_list_node = author_bucket->book_list_head; //
            while (book_list_node) {
                display_book_summary_author(book_list_node->book); //
                found_count++;
                book_list_node = book_list_node->next;
            }
            break; 
        }
        author_bucket = author_bucket->next;
    }
    if (found_count == 0) printf("  └─ 해당 저자의 도서를 찾을 수 없습니다.\n");
    printf("--------------------------\n");
}

void display_books_in_genre_bst(BookNode* bst_root, User* users) {
    if (bst_root == NULL) return;
    display_books_in_genre_bst(bst_root->left, users);
    display_book_details(bst_root, users);
    display_books_in_genre_bst(bst_root->right, users);
}

// 장르 검색 로직 수정 (display_books_in_genre_bst_summary 사용)
void search_books_by_genre_logic(const char* genre_name, const char* sub_genre_name) {
    int index = hash_string(genre_name, HASH_TABLE_SIZE); //
    GenreBucket* genre_bucket = genre_hash_table[index]; //
    int found_any_genre_bucket = 0;
    int found_any_book_in_genre = 0;


    printf("\n--- 장르 '%s'", genre_name);
    if (sub_genre_name && strlen(sub_genre_name) > 0) {
        printf(" (서브장르: '%s')", sub_genre_name);
    }
    printf(" 검색 결과 ---\n");

    while (genre_bucket) {
        if (strcmp(genre_bucket->genre, genre_name) == 0) { //
            found_any_genre_bucket = 1;
            SubGenreNode* sub_node = genre_bucket->sub_genre_list; //
            int printed_genre_header = 0;

            if (sub_node == NULL && !(sub_genre_name && strlen(sub_genre_name) > 0) ) {

            }

            while (sub_node) {
                int match_sub_genre = 0;
                if (sub_genre_name && strlen(sub_genre_name) > 0) { // 특정 서브장르를 찾는 경우
                    if (strcmp(sub_node->sub_genre, sub_genre_name) == 0) { //
                        match_sub_genre = 1;
                    }
                } else { // 특정 서브장르를 찾지 않는 경우 (해당 장르의 모든 서브장르)
                    match_sub_genre = 1;
                }

                if (match_sub_genre) {
                    if (!printed_genre_header) {
                        // printf("└─ 장르: %s\n", genre_bucket->genre); // 이 부분은 이미 위에서 출력
                        printed_genre_header = 1;
                    }
                    if (sub_node->book_bst) { //
                        printf("  ├─ 서브장르: %s\n", sub_node->sub_genre);
                        // *** 수정된 부분: display_books_in_genre_bst_summary 호출 ***
                        display_books_in_genre_bst_summary(sub_node->book_bst);
                        found_any_book_in_genre = 1;
                    } else if (sub_genre_name && strlen(sub_genre_name) > 0) {
                        // 특정 서브장르를 찾았는데 책이 없는 경우
                         printf("  ├─ 서브장르: %s\n", sub_node->sub_genre);
                         printf("  │  └─ 해당 서브장르에 도서가 없습니다.\n");
                         found_any_book_in_genre = 1; // 서브장르 자체는 찾았으므로
                    }
                }
                sub_node = sub_node->next;
            }
            break; 
        }
        genre_bucket = genre_bucket->next;
    }

    if (!found_any_genre_bucket) { // 주 장르 자체를 못 찾은 경우
        printf("  └─ 해당 장르를 찾을 수 없습니다.\n");
    } else if (!found_any_book_in_genre) { // 주 장르는 찾았으나 (또는 특정 서브장르를 지정했으나) 책이 없는 경우
         if (sub_genre_name && strlen(sub_genre_name) > 0) {
             printf("  └─ 지정하신 서브장르를 찾을 수 없거나, 해당 서브장르에 도서가 없습니다.\n");
         } else {
             printf("  └─ 해당 장르에 등록된 도서가 없습니다.\n");
         }
    }
    printf("--------------------------\n");
}

// UI 함수들 (add_new_book_ui, delete_book_ui, search_book_ui, handle_book_management_menu)은
void add_new_book_ui() {
    char title[100], author[50], genre[30], sub_genre[50];
    printf("추가할 도서 제목: "); scanf(" %[^\n]", title);
    printf("저자: "); scanf(" %[^\n]", author);
    printf("장르: "); scanf(" %[^\n]", genre);
    printf("서브 장르: "); scanf(" %[^\n]", sub_genre);
    add_book_logic(title, author, genre, sub_genre);
}

void delete_book_ui() {
    char title[100];
    printf("삭제할 도서 제목: "); scanf(" %[^\n]", title);
    delete_book_logic(title);
}

void search_book_ui(){
    int choice;
    char keyword[100];
    char sub_keyword[50];

    printf("\n--- 도서 검색 옵션 ---\n");
    printf("1. 제목으로 검색\n");
    printf("2. 저자로 검색\n");
    printf("3. 장르로 검색\n");
    printf("0. 이전 메뉴로\n");
    printf("선택: ");
    scanf("%d", &choice);
    while(getchar() != '\n'); 

    switch(choice){
        case 1:
            printf("검색할 제목: "); fgets(keyword, sizeof(keyword), stdin); keyword[strcspn(keyword, "\n")] = 0;
            search_books_by_title_logic(keyword);
            break;
        case 2:
            printf("검색할 저자: "); fgets(keyword, sizeof(keyword), stdin); keyword[strcspn(keyword, "\n")] = 0;
            search_books_by_author_logic(keyword);
            break;
        case 3:
            printf("검색할 장르: "); fgets(keyword, sizeof(keyword), stdin); keyword[strcspn(keyword, "\n")] = 0;
            printf("검색할 서브장르 (없으면 Enter): "); fgets(sub_keyword, sizeof(sub_keyword), stdin); sub_keyword[strcspn(sub_keyword, "\n")] = 0;
            search_books_by_genre_logic(keyword, sub_keyword);
            break;
        case 0: return;
        default: printf("잘못된 선택입니다.\n");
    }
}


void handle_book_management_menu() {
    int choice;
    while (1) {
        printf("\n--- 도서 관리 메뉴 ---\n");
        printf("1. 도서 추가\n");
        printf("2. 도서 삭제\n");
        printf("3. 도서 검색\n");
        printf("0. 메인 메뉴로\n");
        printf("선택: ");

        if (scanf("%d", &choice) != 1) {
            printf("잘못된 입력입니다. 숫자를 입력해주세요.\n");
            while(getchar() != '\n'); 
            continue;
        }
        while(getchar() != '\n'); 

        switch (choice) {
            case 1: add_new_book_ui(); break;
            case 2: delete_book_ui(); break;
            case 3: search_book_ui(); break;
            case 0: return;
            default: printf("잘못된 선택입니다.\n");
        }
    }
}