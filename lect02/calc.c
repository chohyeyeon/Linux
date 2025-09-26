#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

static long long to_ll(const char *s, int *ok) {
    char *end;
    errno = 0;
    long long v = strtoll(s, &end, 10);
    if (errno || *end != '\0') *ok = 0;
    return v;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <num1> <op> <num2>\n", argv[0]);
        fprintf(stderr, "ops: +  -  *  /  %%   (곱셈은 '*' 또는 'x')\n");
        return 1;
    }

    int ok1 = 1, ok2 = 1;
    long long a = to_ll(argv[1], &ok1);
    long long b = to_ll(argv[3], &ok2);
    if (!ok1 || !ok2) {
        fprintf(stderr, "Error: 정수만 입력하세요.\n");
        return 1;
    }

    const char *op = argv[2];

    if (strcmp(op, "+") == 0) {
        printf("%lld\n", a + b);
    } else if (strcmp(op, "-") == 0) {
        printf("%lld\n", a - b);
    } else if (strcmp(op, "*") == 0 || strcmp(op, "x") == 0) {
        printf("%lld\n", a * b);
    } else if (strcmp(op, "/") == 0) {
        if (b == 0) { fprintf(stderr, "Error: 0으로 나눌 수 없습니다.\n"); return 1; }
        printf("%lld\n", a / b);   // 정수 나눗셈
    } else if (strcmp(op, "%") == 0) {
        if (b == 0) { fprintf(stderr, "Error: 0으로 나눌 수 없습니다.\n"); return 1; }
        printf("%lld\n", a % b);
    } else {
        fprintf(stderr, "Unsupported operator: %s\n", op);
        return 1;
    }
    return 0;
}

