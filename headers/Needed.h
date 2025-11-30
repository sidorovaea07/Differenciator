#ifndef NEEDED
#define NEEDED

#include "Colors.h"
#include <stdio.h>

#ifdef DEBUG
    #define PRINT(fmt, ...) printf( YELLOW fmt WHITE , ##__VA_ARGS__ )
    #define PRP(x) printf(PURPLE #x " pointer: %p\n" WHITE, (x))
    #define PRD(x) printf(BLUE #x " = %d\n" WHITE, (x))
    #define PRU(x) printf(BLUE #x " = %lu\n" WHITE, (x))
    #define PRTF(x) printf(PURPLE "print to file returned %d\n" WHITE, (x))
    #define FINISH PRINT( PURPLE "finish\n" WHITE );
    #define PRS(x) printf(BLUE "%s\n" WHITE, x)
#else
    #define PRP(x)
    #define PRS(x)
    #define PRD(x)
    #define PRU(x)
    #define PRTF(x)
    #define PRINT(fmt, ...)
    #define FINISH   
#endif

#endif