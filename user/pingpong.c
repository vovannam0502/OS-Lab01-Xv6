#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int p1[2];
    int p2[2];
    char buf[10]; // Buffer để đọc byte từ pipe

    // Tạo hai pipe p1 và p2
    pipe(p1); // p1 để gửi từ tiến trình cha sang tiến trình con
    pipe(p2); // p2 để gửi từ tiến trình con sang tiến trình cha

    // Tạo tiến trình con
    int pid = fork();

    if (pid < 0)
    {
        printf("error\n");
        exit(0);
    }
    else if (pid == 0) // Tiến trình con
    {
        // Đóng phần ghi của tiến trình cha
        close(p1[1]);
        close(p2[0]);

        // Đọc byte từ tiến trình cha
        read(p1[0], &buf, sizeof(buf));
        printf("%d: received %s\n", getpid(), buf);

        // Gửi lại byte từ tiến trình con sang tiến trình cha
        write(p2[1], "pong", 5);

        close(p1[0]);
        close(p2[1]);
        exit(0);
    }
    else // Tiến trình cha
    {
        // Đóng đầu đọc của pipe p1 và đầu ghi của pipe p2
        close(p1[0]);
        close(p2[1]);

        // Gửi byte từ tiến trình cha sang tiến trình con
        write(p1[1], "ping", 5);
        wait(0);

        // Đọc byte từ tiến trình con
        read(p2[0], buf, sizeof(buf));
        printf("%d: received %s\n", getpid(), buf);

        // Đóng pipe
        close(p1[1]);
        close(p2[0]);
    }

    exit(0);
}
