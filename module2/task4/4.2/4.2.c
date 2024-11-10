#include "queue.h"

int main() {
    PriorityQueue queue;
    initQueue(&queue);

    // Добавление элементов
    enqueue(&queue, 1, 3);
    enqueue(&queue, 2, 1);
    enqueue(&queue, 3, 2);
    enqueue(&queue, 4, 0);
    enqueue(&queue, 5, 12);
    enqueue(&queue, 6, 1);

    printf("Извлечение элемента с наивысшим приоритетом:\n");
    QueueElement* e1 = dequeue(&queue);
    if (e1) {
        printf("Данные: %d, Приоритет: %d\n", e1->data, e1->priority);
        free(e1);
    }

    printf("Извлечение элемента с приоритетом 2:\n");
    QueueElement* e2 = dequeueWithPriority(&queue, 2);
    if (e2) {
        printf("Данные: %d, Приоритет: %d\n", e2->data, e2->priority);
        free(e2);
    }

    printf("Извлечение элемента с приоритетом не ниже 2:\n");
    QueueElement* e3 = dequeueWithMinPriority(&queue, 2);
    if (e3) {
        printf("Данные: %d, Приоритет: %d\n", e3->data, e3->priority);
        free(e3);
    }
    QueueElement* e4 = dequeueWithMinPriority(&queue, 2);
    if (e4) {
        printf("Данные: %d, Приоритет: %d\n", e4->data, e4->priority);
        free(e4);
    }
    QueueElement* e5 = dequeueWithMinPriority(&queue, 2);
    if (e5) {
        printf("Данные: %d, Приоритет: %d\n", e5->data, e5->priority);
        free(e5);
    }

    return 0;
}