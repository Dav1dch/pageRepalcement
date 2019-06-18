#include "struct.h"
#include <cmath>

int main()
{
    replacement process;
    programe p = programe(32767, 320);
    for (int i = 0; i < 5; i++)
    {
        process.pagesize = 1024 * pow(2, i);
        process.transform(p);
        for (int j = 1; j <= (1024 * pow(2, 5)) / process.pagesize; j++)
        {
            process.memNum = j;
            process.FIFO();
            process.LRU();
            process.OPT();
        }
    }

    return 0;
}