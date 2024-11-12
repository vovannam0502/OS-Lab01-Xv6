#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"

int main(int argc, char *argv[]) {
    int index = 1;                  // Vị trí bắt đầu lệnh trong argv
    char *_argv[MAXARG];            // Mảng chứa đối số cho lệnh exec
    char buf[128];                  // Mảng chứa dòng đọc từ stdin
    int _argc = 0;                  // Đếm số lượng đối số cho exec

    if (argc < 2) {
        fprintf(2, "Usage: %s [-n] command [args...]\n", argv[0]);
        exit(1);
    }

    if (strcmp(argv[1], "-n") == 0) {
        index = 3;
    }

    for (int i = index; i < argc; ++i) {
        _argv[_argc++] = argv[i];
    }

    while (1) {
        int len = 0;
        while (read(0, &buf[len], 1) == 1 && buf[len] != '\n') {
            len++;
        }
        if (len == 0) break;
        buf[len] = '\0';

        _argv[_argc] = buf;
        if (fork() == 0) {
            exec(_argv[0], _argv);
            exit(1);
        } else {
            wait(0);
        }
    }

    exit(0);
}
