#ifndef LOAN_MANAGEMENT_H
#define LOAN_MANAGEMENT_H

#include "../init_data/users.h"
#include "../init_data/book_tree.h"
#include "../common/structures.h" // Date 및 BookAndDate 구조체 정의 포함
#include "../common/date_utils.h" // 날짜 함수 프로토타입
#include "../common/queue.h"      // Queue 관련 (BookNode 내 사용)
#include "../common/list.h"       // BookAndDate 리스트 함수 프로토타입

// 전역 변수 선언 (정의는 loan_management.c 에)
extern BookAndDate* global_pickup_list_head;
extern BookAndDate* global_loan_log_head;

// BookAndDate 노드 생성 함수 (정의는 loan_management.c 에)
BookAndDate* create_book_and_date_node(BookNode* book, User* user, Date action_date, Date due_date);

// 핵심 기능 함수 프로토타입
int handle_loan_request(User* current_user, BookNode* requested_book, Date loan_date);
int handle_return_request(User* current_user, BookNode* returned_book, Date return_date);
void process_book_reservation_queue(BookNode* book_to_process);
int process_pickup_loan(User* current_user, BookNode* reserved_book, BookAndDate* pickup_record, Date loan_date);

#define LOAN_DAYS 14
#define PICKUP_EXPIRY_DAYS 3

#endif // LOAN_MANAGEMENT_H