#ifndef USER_MANAGEMENT_H
#define USER_MANAGEMENT_H

#include "../init_data/users.h" // User 구조체 사용

// --- UI 함수 ---
void handle_user_management_menu();
void add_new_user_ui();
void delete_user_ui();
void search_user_ui();
void update_user_info_ui(); // 추가 정보 입력/수정 UI (예: 도서 클럽 가입)

// --- 로직 함수 ---
int add_user_logic(int user_number, const char* name, int is_club_member);
int delete_user_logic(int user_number);
User* search_user_by_number_logic(int user_number);
User* search_user_by_name_logic(const char* name); // 여러 명일 경우 첫 번째 반환 또는 목록 반환 고려
int update_user_club_status_logic(int user_number, int new_status);

// --- 유틸리티 함수 ---
void display_user_details(User* user);

#endif // USER_MANAGEMENT_H