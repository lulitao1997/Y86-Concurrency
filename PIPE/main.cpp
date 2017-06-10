#include "pipe.h"
#include "transfer.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("usage: %s <filepath>\n", argv[0]);
        exit(1);
    }

    transfer(argv[1]);
    pipe_init("__ins_buf__");
    while (!ended) {
        proc_one();
    }
}
