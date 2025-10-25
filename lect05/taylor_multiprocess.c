#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <math.h>
#include <string.h>

#define _USE_MATH_DEFINES
#define MAXLINE 100
#define N 4

void sinx_taylor(int num_elements, int terms, double* x, double* result) {
    int child_id, pid;
    int length;
    char message[MAXLINE], line[MAXLINE];
    int fd[2 * N];

    for (int i = 0; i < num_elements; i++) {
        child_id = i;
        pipe(fd + 2 * i);
        pid = fork();
        if (pid == 0) break;
    }

    if (pid == 0) {
        int i = child_id;
        close(fd[2 * i]);
        double value = x[i];
        double numer = x[i] * x[i] * x[i];
        double denom = 6.0;
        int sign = -1;

        for (int j = 1; j <= terms; j++) {
            value += (double)sign * numer / denom;
            numer *= x[i] * x[i];
            denom *= (2.0 * j + 2.0) * (2.0 * j + 3.0);
            sign *= -1;
        }

        result[i] = value;
        sprintf(message, "%lf", result[i]);
        length = strlen(message) + 1;
        write(fd[2 * i + 1], message, length);
        exit(child_id);
    }

    for (int i = 0; i < num_elements; i++) {
        close(fd[2 * i + 1]);
    }

    for (int i = 0; i < num_elements; i++) {
        int status;
        wait(&status);
        int child = WEXITSTATUS(status);
        int n = read(fd[2 * child], line, MAXLINE);
        result[child] = atof(line);
    }
}

int main() {
    double x[N] = {0.0, M_PI / 6, M_PI / 3, 0.134};
    double res[N];

    sinx_taylor(N, 3, x, res);

    for (int i = 0; i < N; i++) {
        printf("sin(%.3f) by Taylor series = %f\n", x[i], res[i]);
        printf("sin(%.3f) = %f\n", x[i], sin(x[i]));
    }

    return 0;
}

