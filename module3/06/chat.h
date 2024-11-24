#ifndef CHAT
#define CHAT
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>
#define MSG_SIZE 128
#define MSG_KEY 12345
#define MSG_TYPE_PING 1
#define MSG_TYPE_PONG 2
#define MSG_TYPE_EXIT 99
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