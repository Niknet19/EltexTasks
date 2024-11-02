/*
Написать программу, имитирующую принятие решения при
отправке пакета данных по сети с адресацией IPv4. В аргументах запуска
пользователь должен указать IP адрес шлюза, маску подсети и количество
пакетов (N) для имитации обработки. Программа генерирует N случайных
чисел – IP адресов назначения в пакете. Для каждого числа выполняется
преобразование в IP адрес и определяется принадлежность подсети.
После обработки всех пакетов выводится статистика: сколько пакетов (штук и %)
предназначались узлам «своей» подсети, и сколько пакетов
предназначались узлам других сетей.
*/

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

u_int32_t parse_ip(const char* ip_as_str) {
    u_int32_t ip_as_uint = 0;
    long octets[4];
    char* token;
    int i = 0;

    // создаем изменяемую копию строки, с const char* strtok не работает
    char* ip_copy = strdup(ip_as_str);
    token = strtok(ip_copy, ".");
    while (token != NULL && i < 4) {
        octets[i] = atoi(token);
        token = strtok(NULL, ".");
        i++;
    }

    for (size_t i = 0; i < 4; i++) {
        ip_as_uint = ip_as_uint << 8;
        ip_as_uint |= octets[i];
    }
    return ip_as_uint;
}

int ip_check(u_int32_t network_ip, u_int32_t mask, u_int32_t ip_to_check) {
    u_int32_t subnetwork_ip = network_ip & mask;
    u_int32_t subnetwork_checked_ip = ip_to_check & mask;
    return subnetwork_ip == subnetwork_checked_ip;
}

u_int32_t generate_random_ip() {
    return ((u_int32_t)rand() % 256 << 24) | ((u_int32_t)rand() % 256 << 16) | ((u_int32_t)rand() % 256 << 8) |
           ((u_int32_t)rand() % 256);
}

int main(int argc, char* argv[]) {
    u_int32_t gateway_ip = parse_ip(argv[1]);
    u_int32_t subnet_musk_ip = parse_ip(argv[2]);
    int total_ips = atoi(argv[3]);
    int subnet_ips = 0;
    srand((u_int32_t)time(NULL));
    generate_random_ip();
    for (size_t i = 0; i < total_ips; i++) {
        total_ips += ip_check(gateway_ip, subnet_musk_ip, generate_random_ip());
    }
    printf("Принадлежат подсети: %d\n", subnet_ips);
    printf("Всего: %d\n", total_ips);
    printf("Процент: %lf", (double)subnet_ips / (double)total_ips);
    return 0;
}