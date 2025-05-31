#ifndef DATE_UTILS_H
#define DATE_UTILS_H

#include <time.h>
#include "structures.h" // Date 구조체 정의를 위함

// 날짜 관련 유틸리티 함수 프로토타입
Date get_current_date();
Date calculate_return_date(Date loan_date, int loan_days);
Date calculate_pickup_expiry_date(Date start_date, int expiry_days);
int is_date_overdue(Date current_date, Date due_date);
void print_date(Date date);

#endif // DATE_UTILS_H