#pragma once

//    #define OMP_PLACES cores
#define OMP_PLACES threads

#define OMP_WAIT_POLICY = active
//    #define OMP_WAIT_POLICY = passive

#define OMP_DYNAMIC = false

#define OMP_PROC_BIND = true
//    #define OMP_PROC_BIND spread // workers are spread across the available places to maximize the space inbetween two neighbouring threads
//    //#define OMP_PROC_BIND close // worker threads are close to the master in contiguous partitions, e. g. if the master is occupying hardware thread 0, worker 1 will be placed on hw thread 1, worker 2 on hw thread 2 and so on

#include <omp.h>
