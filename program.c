#include <stdio.h>
#include <unistd.h>

int main (int argc, char *argv[])
{
    for (int i = 1; i < argc; i++) {
        printf("Arg %d: %s\n", i, argv[i]);
    }
    return 0;
}