#ifndef PARALLEL_H
#define PARALLEL_H

#include "../Timer/Timer.h"

class ParallelRun
{
public:
    ParallelRun();
    int execute(int argc, char *argv[], Timer *timer);

};

#endif // PARALLEL_H
