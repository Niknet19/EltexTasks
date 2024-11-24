#ifndef CHAT
#define CHAT
#include <fcntl.h>
#include <mqueue.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#define MSG_SIZE 128
#define MAX_QUEUE_SIZE 10
#define MSG_TYPE_PING 1
#define MSG_TYPE_PONG 2
#define QUEUE_NAME "/chat_queue"
#define TRUE 1
#define FALSE 0

typedef struct {
    long mtype;            // Тип сообщения
    char mtext[MSG_SIZE];  // Само сообщение, длиной MSGSZ.
} Message;

void handle_error(char* str) {
    perror(str);
    exit(EXIT_FAILURE);
}

#endif