#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

#define DEFAULT_PORT 1337

int main(int argc, char *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    int port = (argc > 1) ? atoi(argv[1]) : DEFAULT_PORT;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("[DEBUG] [error] socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("[DEBUG] [error] setsockopt failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("[DEBUG] [error] bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0)
    {
        perror("[DEBUG] [error] listen failed");
        exit(EXIT_FAILURE);
    }

    time_t now;
    struct tm *timeinfo;

    time(&now);
    timeinfo = localtime(&now);
    char time_buf[50];
    strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", timeinfo);

    printf("[DEBUG] [info] Listening on port %d... [%s]\n", port, time_buf);

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
    {
        perror("[DEBUG] [error] accept failed");
        exit(EXIT_FAILURE);
    }

    printf("[DEBUG] [info] Connection established [%s]\n", time_buf);

    while (1)
    {
        valread = read(new_socket, buffer, 1024);
        printf("[DEBUG] [info] %s [%s]\n", buffer, time_buf);
        fflush(stdout);
        memset(buffer, 0, sizeof(buffer));
    }

    return 0;
}
