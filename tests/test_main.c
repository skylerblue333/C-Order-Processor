#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

static int exit_code(const char *command) {
    const int status = system(command);
    if (status == -1 || !WIFEXITED(status)) {
        return -1;
    }
    return WEXITSTATUS(status);
}

static int expect_exit(const char *command, int expected) {
    const int actual = exit_code(command);
    if (actual != expected) {
        fprintf(stderr, "command failed expectation: %s (expected %d, got %d)\n", command, expected, actual);
        return 1;
    }
    return 0;
}

int main(void) {
    int failures = 0;
    failures += expect_exit("./app 2500 2 >/tmp/order-no-discount.json", 0);
    failures += expect_exit("grep -q '\"total_cents\":5000' /tmp/order-no-discount.json", 0);
    failures += expect_exit("./app 2500 2 1000 >/tmp/order-discount.json", 0);
    failures += expect_exit("grep -q '\"discount_cents\":500' /tmp/order-discount.json", 0);
    failures += expect_exit("grep -q '\"total_cents\":4500' /tmp/order-discount.json", 0);
    failures += expect_exit("./app 0 1 >/dev/null 2>&1", 2);
    failures += expect_exit("./app 100 0 >/dev/null 2>&1", 2);
    failures += expect_exit("./app 100 1 10001 >/dev/null 2>&1", 2);
    failures += expect_exit("./app nope 1 >/dev/null 2>&1", 2);
    failures += expect_exit("./app 100 -1 >/dev/null 2>&1", 2);
    if (failures != 0) {
        return 1;
    }
    puts("order core contract tests passed");
    return 0;
}
