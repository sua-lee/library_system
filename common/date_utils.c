#include "date_utils.h"
#include <stdio.h>

Date get_current_date() {
    Date d;
    time_t t = time(NULL);
    struct tm tm_info = *localtime(&t);
    d.year = tm_info.tm_year + 1900;
    d.month = tm_info.tm_mon + 1;
    d.day = tm_info.tm_mday;
    return d;
}

Date calculate_return_date(Date loan_date, int loan_days) {
    struct tm t = {0};
    t.tm_year = loan_date.year - 1900;
    t.tm_mon = loan_date.month - 1;
    t.tm_mday = loan_date.day + loan_days;
    mktime(&t);
    Date due_date = {t.tm_year + 1900, t.tm_mon + 1, t.tm_mday};
    return due_date;
}

Date calculate_pickup_expiry_date(Date start_date, int expiry_days) {
    struct tm t = {0};
    t.tm_year = start_date.year - 1900;
    t.tm_mon = start_date.month - 1;
    t.tm_mday = start_date.day + expiry_days;
    mktime(&t);
    Date expiry_date = {t.tm_year + 1900, t.tm_mon + 1, t.tm_mday};
    return expiry_date;
}

int is_date_overdue(Date current_date, Date due_date) {
    if (current_date.year > due_date.year) return 1;
    if (current_date.year == due_date.year && current_date.month > due_date.month) return 1;
    if (current_date.year == due_date.year && current_date.month == due_date.month && current_date.day > due_date.day) return 1;
    return 0;
}

void print_date(Date date) {
    printf("%04d-%02d-%02d", date.year, date.month, date.day);
}