#include "loan_management.h" // BookAndDate 정의는 이제 common/structures.h 를 통해 간접적으로 포함됨
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// common 폴더의 헤더들
#include "../common/structures.h" // BookAndDate 정의 직접 사용
#include "../common/date_utils.h"
#include "../common/queue.h"
#include "../common/list.h"


// 전역 변수 정의
BookAndDate* global_pickup_list_head = NULL;
BookAndDate* global_loan_log_head = NULL;

// BookAndDate 노드 생성 (이제 BookAndDate 정의는 common/structures.h 에 있음)
BookAndDate* create_book_and_date_node(BookNode* book, User* user, Date action_date, Date due_date) {
    BookAndDate* newNode = (BookAndDate*)malloc(sizeof(BookAndDate));
    if (!newNode) {
        perror("BookAndDate Node 생성 실패 (create_book_and_date_node)");
        return NULL;
    }
    newNode->book_information = book;
    newNode->user_information = user;
    newNode->action_date = action_date;
    newNode->due_date = due_date;
    newNode->next = NULL;
    return newNode;
}


// 대출 요청 함수 (handle_loan_request)
int handle_loan_request(User* current_user, BookNode* requested_book, Date loan_date) {
    if (current_user == NULL || requested_book == NULL) {
        printf("오류: 사용자 또는 책 정보가 유효하지 않습니다.\n");
        return 0;
    }

    BookAndDate* pickup_record = find_bad_node_on_pickup_list(global_pickup_list_head, current_user, requested_book); // common/list.c
    if (pickup_record != NULL) {
        return process_pickup_loan(current_user, requested_book, pickup_record, loan_date);
    }

    if (current_user->overdue == 1) {
        printf("알림: 연체 중이므로 대출 및 예약을 할 수 없습니다.\n");
        return 0;
    }
    if (current_user->borrowed_num >= MAX_BORROW_LIMIT) {
        printf("알림: 대출 한도(%d권)를 초과하여 대출할 수 없습니다.\n", MAX_BORROW_LIMIT);
        return 0;
    }

    if (requested_book->is_available_now == 1 && requested_book->reservation_list_num == 0) {
        current_user->borrowed_num++;
        Date return_due_date = calculate_return_date(loan_date, LOAN_DAYS); // common/date_utils.c
        BookAndDate* new_loan = create_book_and_date_node(requested_book, current_user, loan_date, return_due_date);
        if (!new_loan) return 0;
        add_bad_node_to_list(&(current_user->borrowed_book_list_head), new_loan); // common/list.c

        requested_book->is_available_now = 0;

        printf("알림: '%s' 도서 대출 완료. 반납 예정일: ", requested_book->title);
        print_date(return_due_date); // common/date_utils.c
        printf("\n");
        return 1;
    } else {
                // 현재 대출하려는 사용자가 이 책을 이미 빌리고 있는지 먼저 확인
        BookAndDate* current_borrowed_list = current_user->borrowed_book_list_head;
        int already_borrowed_by_user = 0; // 사용자가 이미 대출 중인지 확인하는 플래그

        while (current_borrowed_list != NULL) {
            if (current_borrowed_list->book_information == requested_book) {
                already_borrowed_by_user = 1;
                break;
            }
            current_borrowed_list = current_borrowed_list->next;
        }

        if (already_borrowed_by_user) {
            printf("알림: '%s'님은 현재 '%s' 도서를 이미 대출 중입니다. 예약할 수 없습니다.\n", current_user->name, requested_book->title);
            return 0; // 예약 불가
        }
        // ==================== 수정된 로직 중간 =====================

        // 사용자가 이미 대출 중인 경우가 아니라면, 일반적인 "즉시 대출 불가" 메시지 출력
        printf("알림: '%s' 도서는 현재 즉시 대출이 불가능합니다 (상태: %d, 예약자수: %d).\n",
            requested_book->title, requested_book->is_available_now, requested_book->reservation_list_num);

        if (current_user->reserved_book_node != NULL) {
            if (current_user->reserved_book_node == requested_book) {
                printf("알림: 이미 '%s' 도서를 예약 중입니다.\n", requested_book->title);
            } else {
                printf("알림: 이미 다른 도서('%s')를 예약 중입니다. 한 번에 한 권만 예약 가능합니다.\n", current_user->reserved_book_node->title);
            }
            return 0;
        }

        if (is_user_in_queue(requested_book->reservation_queue, current_user)) { // common/queue.c
             printf("알림: 이미 '%s' 도서의 예약 대기열에 등록되어 있습니다.\n", requested_book->title);
             return 0;
        }

        printf("질문: '%s' 도서를 예약하시겠습니까? (Y/N): ", requested_book->title);
        char confirm_reserve;
        scanf(" %c", &confirm_reserve);
        while(getchar() != '\n');

        if (confirm_reserve == 'Y' || confirm_reserve == 'y') {
            enqueue_user(requested_book->reservation_queue, current_user); // common/queue.c
            requested_book->reservation_list_num++;
            current_user->reserved_book_node = requested_book;
            printf("알림: '%s' 도서 예약 완료. 현재 대기 순번: %d\n", requested_book->title, requested_book->reservation_list_num);
            return 1;
        } else {
            printf("알림: 예약을 취소했습니다.\n");
            return 0;
        }
    }
    return 0;
}

// 반납 요청 함수 (handle_return_request)
int handle_return_request(User* current_user, BookNode* returned_book, Date return_date) {
    if (current_user == NULL || returned_book == NULL) {
        printf("오류: 사용자 또는 책 정보가 유효하지 않습니다.\n");
        return 0;
    }

    BookAndDate* loan_record = find_and_remove_bad_node_by_book(&(current_user->borrowed_book_list_head), returned_book); // common/list.c

    if (loan_record == NULL) {
        printf("오류: 해당 사용자는 '%s' 도서를 대출하지 않았습니다.\n", returned_book->title);
        return 0;
    }

    current_user->borrowed_num--;
    printf("알림: '%s' 도서 반납 처리되었습니다.\n", returned_book->title);

    if (is_date_overdue(return_date, loan_record->due_date)) { // common/date_utils.c
        printf("경고: '%s' 도서가 연체되었습니다. 반납 예정일: ", returned_book->title);
        print_date(loan_record->due_date); // common/date_utils.c
        printf(", 반납일: ");
        print_date(return_date); // common/date_utils.c
        printf("\n");

        current_user->overdue = 1;
        add_bad_node_to_list(&(current_user->overdue_book_list_head), loan_record); // common/list.c

        if (current_user->reserved_book_node != NULL) {
            BookNode* previously_reserved_book = current_user->reserved_book_node;
            if (remove_user_from_queue(previously_reserved_book->reservation_queue, current_user)) { // common/queue.c
                previously_reserved_book->reservation_list_num--;
            }
            current_user->reserved_book_node = NULL;
            printf("알림: 연체로 인해 예약 중이던 '%s' 도서의 예약이 자동 취소되었습니다.\n", previously_reserved_book->title);
        }
    } else {
        free_bad_node(loan_record); // common/list.c
    }

    if (current_user->overdue == 1 && current_user->overdue_book_list_head == NULL) {
        printf("알림: 모든 연체 도서가 반납되어 연체 상태가 해제됩니다.\n");
        current_user->overdue = 0;
    }

    process_book_reservation_queue(returned_book);
    return 1;
}

// 예약 대기열 처리 함수 (process_book_reservation_queue)
void process_book_reservation_queue(BookNode* book_to_process) {
    if (book_to_process == NULL) return;

    if (is_queue_empty(book_to_process->reservation_queue)) { // common/queue.c
        book_to_process->is_available_now = 1;
        return;
    }

    User* next_eligible_user = NULL;
    int initial_queue_size = get_queue_size(book_to_process->reservation_queue); // common/queue.c

    for (int i = 0; i < initial_queue_size; ++i) {
        User* temp_user_holder = peek_front_user(book_to_process->reservation_queue); // common/queue.c
        if (!temp_user_holder) break;

        if (temp_user_holder->overdue == 0 && temp_user_holder->borrowed_num < MAX_BORROW_LIMIT) {
            next_eligible_user = dequeue_user(book_to_process->reservation_queue); // common/queue.c
            break;
        } else {
            User* ineligible_user = dequeue_user(book_to_process->reservation_queue); // common/queue.c
            enqueue_user(book_to_process->reservation_queue, ineligible_user); // common/queue.c
            printf("알림: 예약자 '%s'님은 현재 대출 자격 미달입니다. 다음 순번으로 넘어갑니다.\n", ineligible_user->name);
        }
    }
    book_to_process->reservation_list_num = get_queue_size(book_to_process->reservation_queue); // common/queue.c


    if (next_eligible_user != NULL) {
        Date pickup_start_date = get_current_date(); // common/date_utils.c
        Date pickup_expiry_date = calculate_pickup_expiry_date(pickup_start_date, PICKUP_EXPIRY_DAYS); // common/date_utils.c
        BookAndDate* pickup_entry = create_book_and_date_node(book_to_process, next_eligible_user, pickup_start_date, pickup_expiry_date);
        if (!pickup_entry) {
            enqueue_user(book_to_process->reservation_queue, next_eligible_user); // common/queue.c
            book_to_process->reservation_list_num++;
            return;
        }
        add_bad_node_to_list(&global_pickup_list_head, pickup_entry); // common/list.c
        book_to_process->is_available_now = 2;

        printf("알림: 다음 예약자 '%s'님에게 '%s' 도서 수령 안내. 수령 마감일: ", next_eligible_user->name, book_to_process->title);
        print_date(pickup_expiry_date); // common/date_utils.c
        printf("\n");
    } else {
        book_to_process->is_available_now = 1;
        printf("알림: '%s' 도서의 예약 대기열에 현재 수령 가능한 사용자가 없습니다. 도서가 대출 가능 상태로 변경됩니다.\n", book_to_process->title);
    }
}

// 수령 대기자 전용 대출 함수 (process_pickup_loan)
int process_pickup_loan(User* current_user, BookNode* reserved_book, BookAndDate* pickup_record_in_global_list, Date loan_date) {
    if (is_date_overdue(loan_date, pickup_record_in_global_list->due_date)) { // common/date_utils.c
        printf("오류: '%s' 도서 수령 기간(", reserved_book->title);
        print_date(pickup_record_in_global_list->due_date); // common/date_utils.c
        printf(")이 만료되었습니다.\n");

        if (current_user->reserved_book_node == reserved_book) {
            current_user->reserved_book_node = NULL;
        }
        remove_bad_node_from_list_by_data(&global_pickup_list_head, pickup_record_in_global_list); // common/list.c
        free_bad_node(pickup_record_in_global_list); // common/list.c
        process_book_reservation_queue(reserved_book);
        return 0;
    }

    if (current_user->overdue == 1) {
        printf("오류: 연체 중이므로 예약된 도서('%s')를 대출할 수 없습니다. 예약이 자동 취소됩니다.\n", reserved_book->title);
        if (current_user->reserved_book_node == reserved_book) {
            current_user->reserved_book_node = NULL;
        }
        remove_bad_node_from_list_by_data(&global_pickup_list_head, pickup_record_in_global_list); // common/list.c
        free_bad_node(pickup_record_in_global_list); // common/list.c
        process_book_reservation_queue(reserved_book);
        return 0;
    }
    if (current_user->borrowed_num >= MAX_BORROW_LIMIT) {
        printf("오류: 대출 한도(%d권)를 초과하여 예약된 도서('%s')를 대출할 수 없습니다. 예약은 유지되나 수령기간 내 조건 만족 시 가능합니다.\n", MAX_BORROW_LIMIT, reserved_book->title);
        return 0;
    }

    current_user->borrowed_num++;
    Date return_due_date = calculate_return_date(loan_date, LOAN_DAYS); // common/date_utils.c
    BookAndDate* new_loan = create_book_and_date_node(reserved_book, current_user, loan_date, return_due_date);
    if (!new_loan) return 0;
    add_bad_node_to_list(&(current_user->borrowed_book_list_head), new_loan); // common/list.c

    reserved_book->is_available_now = 0;
    if (current_user->reserved_book_node == reserved_book) {
        current_user->reserved_book_node = NULL;
    }

    remove_bad_node_from_list_by_data(&global_pickup_list_head, pickup_record_in_global_list); // common/list.c
    free_bad_node(pickup_record_in_global_list); // common/list.c

    printf("알림: 예약 도서 '%s' 수령 및 대출 완료. 반납 예정일: ", reserved_book->title);
    print_date(return_due_date); // common/date_utils.c
    printf("\n");
    return 1;
}