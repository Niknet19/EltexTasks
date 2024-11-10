#define MAX_QUEUE_SIZE 100
#include <stdio.h>
#include <stdlib.h>
// Структура для элемента очереди
typedef struct QueueElement {
    int data;                   // Данные сообщения
    int priority;               // Приоритет (от 0 до 255, 0 - наивысший)
    struct QueueElement* next;  // Указатель на следующий элемент
    struct QueueElement* prev;  // Указатель на предыдущий элемент
} QueueElement;

// Структура очереди
typedef struct {
    QueueElement* head;  // Указатель на первый элемент очереди
    QueueElement* tail;  // Указатель на последний элемент очереди
} PriorityQueue;

void initQueue(PriorityQueue* queue);
void enqueue(PriorityQueue* queue, int data, int priority);
QueueElement* dequeue(PriorityQueue* queue);
QueueElement* dequeueWithPriority(PriorityQueue* queue, int priority);
QueueElement* dequeueWithMinPriority(PriorityQueue* queue, int minPriority);