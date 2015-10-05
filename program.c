#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

//todo: better name for this function
int fileIsEven (char* filename) {
    //version 1: use 'stat' to find the length.
    //the length is the amount of characters
    struct stat sb;
    if(stat(filename, &sb) == 0) {
        //failed to read file
        int err = errno;
        printf("error reading %s: %d\n\n", filename, err);
        return -1;
    } else if(!S_ISREG(sb.st_mode)) {
        //not a file (directory?)
        return -1;
    } else {
        //is a file
        //if even, modulus 2 will give 0
        //if odd, will give 1
        return sb.st_size % 2 == 0;
    }
}

int main (int argc, char *argv[])
{
    //arguments start at argv[1] to argv[argc-1]
    for (int i = 1; i < argc; i++) {
        char* filename = argv[i];
        printf("Arg %d: %s Even? %d\n", i, filename, fileIsEven(filename));
    }
    return 0;
}