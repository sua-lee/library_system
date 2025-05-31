#include "bestseller.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // strcmp 사용 위함
#include "../init_data/book_tree.h" // BookNode 구조체

// 임시로 도서 빈도수를 저장하기 위한 구조체
typedef struct BookFrequency {
    BookNode* book;
    int count;
    char title[100]; // 정렬 후 원본 BookNode 없이도 제목을 알 수 있게 하기 위함 (선택적)
} BookFrequency;

// qsort 비교 함수 (빈도수 내림차순)
int compare_book_frequency(const void* a, const void* b) {
    BookFrequency* bf_a = (BookFrequency*)a;
    BookFrequency* bf_b = (BookFrequency*)b;
    return (bf_b->count - bf_a->count); // 내림차순
}

// 스택 순회 시 빈도수 계산을 위한 콜백 함수에 전달될 컨텍스트 구조체
typedef struct AnalysisContext {
    BookFrequency* frequencies;
    int* unique_books_count;
    int* capacity;
} AnalysisContext;

// traverse_book_stack_for_analysis 에 전달될 콜백 함수
void count_book_frequency_callback(BookNode* book, void* context) {
    AnalysisContext* ctx = (AnalysisContext*)context;
    int found = 0;
    for (int i = 0; i < *(ctx->unique_books_count); i++) {
        // BookNode 포인터 주소로 비교 (동일한 책 객체인지)
        if (ctx->frequencies[i].book == book) {
            ctx->frequencies[i].count++;
            found = 1;
            break;
        }
    }
    if (!found) {
        if (*(ctx->unique_books_count) >= *(ctx->capacity)) {
            *(ctx->capacity) *= 2;
            BookFrequency* temp_freq = (BookFrequency*)realloc(ctx->frequencies, (*(ctx->capacity)) * sizeof(BookFrequency));
            if (!temp_freq) {
                fprintf(stderr, "오류: 빈도수 배열 용량 확장 실패 (callback)\n");
                // realloc 실패 시 기존 frequencies는 유효하므로, 더 이상 추가하지 않도록 처리 필요
                // 또는 프로그램 종료 등의 심각한 오류 처리
                return; // 더 이상 추가 불가
            }
            ctx->frequencies = temp_freq; // realloc 성공 시 포인터 업데이트
        }
        ctx->frequencies[*(ctx->unique_books_count)].book = book;
        ctx->frequencies[*(ctx->unique_books_count)].count = 1;
        strncpy(ctx->frequencies[*(ctx->unique_books_count)].title, book->title, 99);
        ctx->frequencies[*(ctx->unique_books_count)].title[99] = '\0';
        (*(ctx->unique_books_count))++;
    }
}


void display_bestsellers(BookStack* activity_stack, int top_n) {
    if (is_book_stack_empty(activity_stack)) {
        printf("최근 활동 기록이 없어 베스트셀러를 표시할 수 없습니다.\n");
        return;
    }

    printf("\n--- 최근 활동 기반 베스트셀러 TOP %d ---\n", top_n);

    AnalysisContext context;
    context.unique_books_count = (int*)malloc(sizeof(int));
    context.capacity = (int*)malloc(sizeof(int));

    if (!context.unique_books_count || !context.capacity) {
        fprintf(stderr, "메모리 할당 오류 (context)\n");
        free(context.unique_books_count);
        free(context.capacity);
        return;
    }

    *(context.unique_books_count) = 0;
    *(context.capacity) = 10; // 초기 용량
    context.frequencies = (BookFrequency*)malloc(*(context.capacity) * sizeof(BookFrequency));

    if (!context.frequencies) {
        fprintf(stderr, "오류: 베스트셀러 빈도수 계산을 위한 메모리 할당 실패\n");
        free(context.unique_books_count);
        free(context.capacity);
        return;
    }

    // 스택을 순회하며 빈도수 계산
    traverse_book_stack_for_analysis(activity_stack, count_book_frequency_callback, &context);

    // 정렬 (realloc이 발생했을 수 있으므로, context.frequencies를 사용)
    qsort(context.frequencies, *(context.unique_books_count), sizeof(BookFrequency), compare_book_frequency);

    int count_to_display = (*(context.unique_books_count) < top_n) ? *(context.unique_books_count) : top_n;
    for (int i = 0; i < count_to_display; i++) {
        printf("%d. 제목: %s (활동 횟수: %d)\n",
               i + 1, context.frequencies[i].title, context.frequencies[i].count);
    }

    if (count_to_display == 0) {
         printf("표시할 베스트셀러 정보가 없습니다.\n");
    }

    free(context.frequencies);
    free(context.unique_books_count);
    free(context.capacity);
    printf("-------------------------------------\n");
}