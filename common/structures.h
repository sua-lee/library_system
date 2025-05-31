#ifndef STRUCTURES_H
#define STRUCTURES_H

// Forward declarations for pointers within structures
struct User;
struct BookNode;

// 날짜 구조체
typedef struct Date {
    int year;
    int month;
    int day;
} Date;

// 대출/반납/수령대기 기록용 구조체 (핵심 공유 구조체)
typedef struct BookAndDate {
    Date action_date;       // 대출일 또는 수령 시작일
    Date due_date;          // 반납 예정일 또는 수령 마감일

    struct BookNode* book_information;
    struct User* user_information;
    struct BookAndDate* next;
} BookAndDate;

#endif // STRUCTURES_H