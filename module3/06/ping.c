#include "chat.h"

int msgid;

void sigint_handler(int sig) {
    //чтобы очереди удалялись при нажатии ctrl + c
    printf("\nПолучен сигнал SIGINT (Ctrl+C). Завершаю работу...\n");
    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
        handle_error("msgctl");
    } else {
        fprintf(stdout, "Очередь успешно удалена.\n");
    }
    exit(EXIT_SUCCESS);
}

int send_message(Message* message, int msgid, long send_type, const char* role) {
    message->mtype = send_type;
    fprintf(stdout, "%s : %s", role, "Введите сообщение: ");
    if (fgets(message->mtext, MSG_SIZE, stdin) == NULL) {
        perror("Ошибка чтения ввода");
        return EXIT_FAILURE;
    }
    message->mtext[strcspn(message->mtext, "\n")] = '\0';

    if (msgsnd(msgid, message, MSG_SIZE, 0) == -1) {
        handle_error("msgsnd");
    }

    if (strcmp(message->mtext, "exit") == 0) {
        fprintf(stdout, "%s : %s", role, "Завершение работы.\n");
        exit(EXIT_SUCCESS);
        // exit_flag = 1;
    }
    return EXIT_SUCCESS;
}

int recieve_message(Message* message, int msgid, long recieve_type, const char* role) {
    if (msgrcv(msgid, message, MSG_SIZE, recieve_type, 0) == -1) {
        handle_error("msgsnd");
    }
    fprintf(stdout, "%s : %s %s\n", role, "Получено сообщение: ", message->mtext);

    if (strcmp(message->mtext, "exit") == 0) {
        fprintf(stdout, "%s : %s", role, "Завершение работы.\n");
        if (msgctl(msgid, IPC_RMID, NULL) == -1) {
            handle_error("msgctl");
        } else {
            fprintf(stdout, "%s : %s", role, "Очередь успешно удалена.\n");
        }
        exit(EXIT_SUCCESS);
        // exit_flag = 1;
    }
    return EXIT_SUCCESS;
}

// void exchange_messages(pid_t pid, int msgid, Message* message, long send_type, long receive_type) {}

int main() {
    key_t key = ftok("chat.h", 2);
    msgid = msgget(key, IPC_CREAT | 0666);
    Message message;

    if (msgid == -1) {
        handle_error("msgid");
    }
    fprintf(stdout, "%s", "Очередь сообщений создана.\n");
    pid_t pid = fork();
    if (pid < 0) {
        perror("Ошибка fork");
    }

    if (pid > 0) {
        if (signal(SIGINT, sigint_handler) == SIG_ERR) {
            perror("Не удалось установить обработчик сигнала");
            exit(1);
        }

        // printf("parent: %d\n", getpid());
        // fflush(stdout);
        while (TRUE) {
            send_message(&message, msgid, MSG_TYPE_PING, "Ping");
            // msgrcv
            recieve_message(&message, msgid, MSG_TYPE_PONG, "Ping");
        }
    }
    if (pid == 0) {
        // printf("child: %d\n", getpid());
        // fflush(stdout);
        while (TRUE) {
            recieve_message(&message, msgid, MSG_TYPE_PING, "Pong");

            send_message(&message, msgid, MSG_TYPE_PONG, "Pong");
        }
    }
}