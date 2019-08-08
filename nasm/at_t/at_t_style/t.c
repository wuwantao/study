#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    char *str = "he\n";
    syscall(1, 2, str, 3);
    return;
}

