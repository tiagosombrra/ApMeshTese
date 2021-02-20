#ifndef MPIRUN_H
#define MPIRUN_H

#include "../Timer/Timer.h"

class MpiRun
{
public:
    MpiRun();
    int execute(int argc, char *argv[], Timer *timer);


};

#endif // MPIRUN_H
