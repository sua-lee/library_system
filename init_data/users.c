#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "users.h"

// 사용자 노드 생성
User* create_user(int number, const char* name) {
    User* new_user = (User*)malloc(sizeof(User));
    if (!new_user) {
        fprintf(stderr, "메모리 할당 실패\n");
        exit(1);
    }

    new_user->number = number;
    strncpy(new_user->name, name, MAX_NAME_LEN - 1);
    new_user->name[MAX_NAME_LEN - 1] = '\0'; // 안전한 널 종료
    new_user->next = NULL;

    return new_user;
}

// 사용자 노드를 리스트 앞쪽에 삽입
void insert_user(User** head, User* new_user) {
    if (!head || !new_user) return;
    new_user->next = *head;
    *head = new_user;
}

// CSV 파일로부터 사용자 리스트 구성
void load_users(const char* filename, User** head) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("파일 열기 실패");
        return;
    }

    char line[256];
    fgets(line, sizeof(line), file); // 헤더 스킵

    while (fgets(line, sizeof(line), file)) {
        int number;
        char name[MAX_NAME_LEN];

        line[strcspn(line, "\r\n")] = '\0';  // 개행 제거

        char* token = strtok(line, ",");
        if (!token) continue;
        number = atoi(token);

        token = strtok(NULL, ",");
        if (!token) continue;
        strncpy(name, token, MAX_NAME_LEN - 1);
        name[MAX_NAME_LEN - 1] = '\0';

        User* user = create_user(number, name);
        insert_user(head, user);
    }

    fclose(file);
}

// 사용자 리스트 출력 (디버깅용)
void print_users(User* head) {
    printf("사용자 목록:\n");
    while (head) {
        printf("번호: %d, 이름: %s\n", head->number, head->name);
        head = head->next;
    }
}

User* find_user_by_number(User* head, int user_number) {
    User* current = head;
    while (current != NULL) {
        if (current->number == user_number) {
            return current; // 사용자를 찾음
        }
        current = current->next;
    }
    return NULL; // 해당 번호의 사용자를 찾지 못함
}

// 이름으로 찾는 함수도 필요하다면 여기에 구현
User* find_user_by_name(User* head, const char* user_name) {
    User* current = head;
    while (current != NULL) {
        if (strcmp(current->name, user_name) == 0) { // 문자열 비교
            return current; // 첫 번째로 일치하는 사용자 반환
        }
        current = current->next;
    }
    return NULL;
}