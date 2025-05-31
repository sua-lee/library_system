#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

Queue* create_queue() {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    if (!q) {
        perror("Queue 생성 실패");
        exit(EXIT_FAILURE);
    }
    q->front = q->rear = NULL;
    q->count = 0;
    return q;
}

void enqueue_user(Queue* q, User* user) {
    if (!q || !user) return;
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    if (!newNode) {
        perror("QueueNode 생성 실패 (enqueue_user)");
        return;
    }
    newNode->user_data = user;
    newNode->next = NULL;
    if (is_queue_empty(q)) {
        q->front = q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
    q->count++;
}

User* dequeue_user(Queue* q) {
    if (is_queue_empty(q) || !q->front) return NULL;
    QueueNode* temp = q->front;
    User* userData = temp->user_data;
    q->front = q->front->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }
    free(temp);
    q->count--;
    return userData;
}

User* peek_front_user(Queue* q) {
    if (is_queue_empty(q) || !q->front) return NULL;
    return q->front->user_data;
}

int is_queue_empty(Queue* q) {
    return (q == NULL || q->count == 0);
}

int get_queue_size(Queue* q) {
    if (!q) return 0;
    return q->count;
}

int is_user_in_queue(Queue* q, User* user_to_find) {
    if (is_queue_empty(q) || !user_to_find) return 0;
    QueueNode* current = q->front;
    while (current) {
        if (current->user_data == user_to_find) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

int remove_user_from_queue(Queue* q, User* user_to_remove) {
    if (is_queue_empty(q) || !user_to_remove) return 0;

    QueueNode* current = q->front;
    QueueNode* prev = NULL;

    while (current != NULL) {
        if (current->user_data == user_to_remove) {
            if (prev == NULL) {
                q->front = current->next;
                if (q->front == NULL) q->rear = NULL;
            } else {
                prev->next = current->next;
                if (prev->next == NULL) q->rear = prev;
            }
            free(current);
            q->count--;
            return 1;
        }
        prev = current;
        current = current->next;
    }
    return 0;
}

void free_queue(Queue* q) {
    if (!q) return;
    QueueNode* current = q->front;
    QueueNode* next_node;
    while (current != NULL) {
        next_node = current->next;
        free(current);
        current = next_node;
    }
    free(q);
}