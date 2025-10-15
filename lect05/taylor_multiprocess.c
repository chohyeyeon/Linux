#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <math.h>

#define N 4
#define TERMS 3

double sinx_taylor(double x, int terms) {
    double sum = x;
    double power = x * x * x;
    double fact = 6.0;
    int s = -1;

    for(int k = 1; k <= terms; k++) {
        sum += s * (power / fact);
        power *= x * x;
        fact *= (2.0*k + 2.0) * (2.0*k + 3.0);
        s = -s;
    }
    return sum;
}

int main() {
    double x[N] = {0, M_PI/6., M_PI/3., 0.134};
    double result[N];
    int fd[N][2];

    for(int i=0; i<N; i++) {
        if(pipe(fd[i]) == -1) {
            perror("pipe");
            exit(1);
        }
    }
    for(int i=0; i<N; i++) {
        pid_t pid = fork();
        if(pid < 0) {
            perror("fork");
            exit(1);
        }
        if(pid == 0) {
            close(fd[i][0]);
            double res = sinx_taylor(x[i], TERMS);
            write(fd[i][1], &res, sizeof(double));
            close(fd[i][1]);
            exit(0);
        } else {
            close(fd[i][1]);
        }
    }

    for(int i=0; i<N; i++) {
        wait(NULL);
    }

    for(int i=0; i<N; i++) {
        read(fd[i][0], &result[i], sizeof(double));
        close(fd[i][0]);
    }
    for(int i=0; i<N; i++) {
        printf("sin(%.3f) by Taylor series = %.6f\n", x[i], result[i]);
        printf("sin(%.3f) = %.6f\n", x[i], sin(x[i]));
    }

    return 0;
}

