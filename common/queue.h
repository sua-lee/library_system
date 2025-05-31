#ifndef QUEUE_H
#define QUEUE_H

#include "../init_data/users.h" // User 구조체 사용을 위해

typedef struct QueueNode {
    User* user_data;
    struct QueueNode* next;
} QueueNode;

typedef struct Queue {
    QueueNode* front;
    QueueNode* rear;
    int count;
} Queue;

Queue* create_queue();
void enqueue_user(Queue* q, User* user);
User* dequeue_user(Queue* q);
User* peek_front_user(Queue* q);
int is_queue_empty(Queue* q);
int get_queue_size(Queue* q);
int is_user_in_queue(Queue* q, User* user_to_find);
int remove_user_from_queue(Queue* q, User* user_to_remove);
void free_queue(Queue* q);

#endif // QUEUE_H