#include "chat.h"

mqd_t msgid;  // Идентификатор очереди

void sigint_handler(int sig) {
    // Чтобы очередь сообщений удалялась при нажатии Ctrl+C
    printf("\nПолучен сигнал SIGINT (Ctrl+C). Завершаю работу...\n");
    if (mq_close(msgid) == -1) {
        perror("mq_close");
    } else {
        fprintf(stdout, "Очередь успешно закрыта.\n");
    }
    if (mq_unlink(QUEUE_NAME) == -1) {
        perror("mq_unlink");
    } else {
        fprintf(stdout, "Очередь успешно удалена.\n");
    }
    exit(EXIT_SUCCESS);
}

int send_message(char* message, mqd_t msgid, const char* role) {
    fprintf(stdout, "%s : %s", role, "Введите сообщение: ");
    if (fgets(message, MSG_SIZE, stdin) == NULL) {
        perror("Ошибка чтения ввода");
        return EXIT_FAILURE;
    }
    message[strcspn(message, "\n")] = '\0';

    if (mq_send(msgid, message, MSG_SIZE, 0) == -1) {
        perror("mq_send");
        return EXIT_FAILURE;
    }

    if (strcmp(message, "exit") == 0) {
        fprintf(stdout, "%s : %s", role, "Завершение работы.\n");
        exit(EXIT_SUCCESS);
    }
    return EXIT_SUCCESS;
}

int recieve_message(char* message, mqd_t msgid, const char* role) {
    ssize_t bytes_received = mq_receive(msgid, message, MSG_SIZE, NULL);
    if (bytes_received == -1) {
        perror("mq_receive");
        return EXIT_FAILURE;
    }
    fprintf(stdout, "%s : %s %s\n", role, "Получено сообщение: ", message);

    if (strcmp(message, "exit") == 0) {
        fprintf(stdout, "%s : %s", role, "Завершение работы.\n");
        if (mq_close(msgid) == -1) {
            perror("mq_close");
        }
        if (mq_unlink(QUEUE_NAME) == -1) {
            perror("mq_unlink");
        }
        exit(EXIT_SUCCESS);
    }
    return EXIT_SUCCESS;
}

int main() {
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_QUEUE_SIZE;
    attr.mq_msgsize = MSG_SIZE;
    attr.mq_curmsgs = 0;

    // Создание очереди сообщений с указанием атрибутов
    msgid = mq_open(QUEUE_NAME, O_CREAT | O_RDWR, 0666, &attr);
    if (msgid == (mqd_t)-1) {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    fprintf(stdout, "Очередь сообщений создана.\n");

    pid_t pid = fork();
    if (pid < 0) {
        perror("Ошибка fork");
        exit(EXIT_FAILURE);
    }

    char message[MSG_SIZE];

    if (pid > 0) {  // Родительский процесс
        if (signal(SIGINT, sigint_handler) == SIG_ERR) {
            perror("Не удалось установить обработчик сигнала");
            exit(1);
        }
        while (TRUE) {
            send_message(message, msgid, "Ping");
            recieve_message(message, msgid, "Ping");
        }
    }

    if (pid == 0) {  // Дочерний процесс
        while (TRUE) {
            recieve_message(message, msgid, "Pong");
            send_message(message, msgid, "Pong");
        }
    }

    return 0;
}
