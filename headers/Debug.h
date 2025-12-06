#ifndef HEADERS_DEBUG
#define HEADERS_DEBUG

#include "Colors.h"
#include <stdio.h>

#ifdef DEBUG
    #define PRINT(fmt, ...) printf( YELLOW fmt WHITE , ##__VA_ARGS__ )
    #define PRP(x) printf(PURPLE #x " pointer: %p\n" WHITE, (x))
    #define PRD(x) printf(BLUE #x " = %d\n" WHITE, (x))
    #define PRU(x) printf(BLUE #x " = %lu\n" WHITE, (x))
    #define PRTF(x) printf(PURPLE "print to file returned %d\n" WHITE, (x))
    #define FINISH PRINT( PURPLE "FINISH\n" WHITE );
    #define PRS(x) printf(BLUE #x ": %s" WHITE, x)
    #define DO(x) x
#else
    #define PRP(x)
    #define PRS(x)
    #define PRD(x)
    #define PRU(x)
    #define PRTF(x)
    #define PRINT(fmt, ...)
    #define FINISH
    #define DO(x)   
#endif

#endif