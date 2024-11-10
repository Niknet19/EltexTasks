#include "queue.h"

void initQueue(PriorityQueue* queue) {
    queue->head = NULL;
    queue->tail = NULL;
}

// Функция добавления элемента в очередь
void enqueue(PriorityQueue* queue, int data, int priority) {
    QueueElement* newElement = (QueueElement*)malloc(sizeof(QueueElement));
    newElement->data = data;
    newElement->priority = priority;
    newElement->next = NULL;
    newElement->prev = NULL;

    if (queue->head == NULL) {
        // Если очередь пуста, элемент становится первым
        queue->head = queue->tail = newElement;
    } else {
        // Вставляем элемент в очередь с учетом приоритета
        QueueElement* current = queue->head;
        while (current != NULL && current->priority <= priority) {
            current = current->next;
        }

        if (current == NULL) {
            // Вставка в конец очереди
            queue->tail->next = newElement;
            newElement->prev = queue->tail;
            queue->tail = newElement;
        } else if (current == queue->head) {
            // Вставка в начало очереди
            newElement->next = queue->head;
            queue->head->prev = newElement;
            queue->head = newElement;
        } else {
            // Вставка в середину очереди
            newElement->next = current;
            newElement->prev = current->prev;
            current->prev->next = newElement;
            current->prev = newElement;
        }
    }
}

// Функция извлечения элемента с наивысшим приоритетом
QueueElement* dequeue(PriorityQueue* queue) {
    if (queue->head == NULL) {
        printf("Очередь пуста!\n");
        return NULL;
    }

    QueueElement* highestPriorityElement = queue->head;
    queue->head = queue->head->next;

    if (queue->head != NULL) {
        queue->head->prev = NULL;
    } else {
        queue->tail = NULL;  // Если очередь пуста
    }

    return highestPriorityElement;
}

// Функция извлечения элемента с заданным приоритетом
QueueElement* dequeueWithPriority(PriorityQueue* queue, int priority) {
    QueueElement* current = queue->head;

    while (current != NULL) {
        if (current->priority == priority) {
            if (current->prev != NULL) {
                current->prev->next = current->next;
            } else {
                queue->head = current->next;  // Если это первый элемент
            }

            if (current->next != NULL) {
                current->next->prev = current->prev;
            } else {
                queue->tail = current->prev;  // Если это последний элемент
            }

            return current;
        }
        current = current->next;
    }

    printf("Элемент с приоритетом %d не найден.\n", priority);
    return NULL;
}

// Функция извлечения элемента с приоритетом не ниже заданного
QueueElement* dequeueWithMinPriority(PriorityQueue* queue, int minPriority) {
    QueueElement* current = queue->head;

    while (current != NULL) {
        if (current->priority <= minPriority) {
            if (current->prev != NULL) {
                current->prev->next = current->next;
            } else {
                queue->head = current->next;  // Если это первый элемент
            }

            if (current->next != NULL) {
                current->next->prev = current->prev;
            } else {
                queue->tail = current->prev;  // Если это последний элемент
            }

            return current;
        }
        current = current->next;
    }

    printf("Элемент с приоритетом не ниже %d не найден.\n", minPriority);
    return NULL;
}