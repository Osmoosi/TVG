#ifndef _VL_TIMEMEASURE_H_
#define _VL_TIMEMEASURE_H_

#include <stdio.h>

#ifdef _WIN32
#pragma warning(push, 0)
#include <windows.h>
#pragma warning(pop)
#else
#include <sys/time.h>
#endif

#ifdef DO_TIMING
#ifdef _WIN32
typedef LONGLONG timemeasure_t;
#else
typedef struct timeval timemeasure_t;
#endif
#else
typedef int timemeasure_t;
#endif

static inline timemeasure_t begin_timing()
{
#ifdef DO_TIMING
#ifdef _WIN32
    LONGLONG result;
    QueryPerformanceCounter((PLARGE_INTEGER)&result);
    return result;
#else
    struct timeval result;
    gettimeofday(&result, NULL);
    return result;
#endif
#else
    return 0;
#endif
}

static inline double end_timing(timemeasure_t timer, const char *name)
{
    double result = 0.0;

#ifdef DO_TIMING
#ifdef _WIN32
    LONGLONG start = timer;
    LONGLONG end = begin_timing();
    LONGLONG ticks_per_sec;
    QueryPerformanceFrequency((PLARGE_INTEGER)&ticks_per_sec);
    result = (double)(end - start) / ticks_per_sec;
#else
    timemeasure_t end = begin_timing();
    end.tv_sec -= timer.tv_sec;
    end.tv_usec -= timer.tv_usec;
    
    if (end.tv_usec < 0)
    {
        end.tv_usec += 1000000;
        end.tv_sec -= 1;
    }
    
    result = end.tv_sec + end.tv_usec / 1000000.0;
#endif
    
    //fprintf(stderr, "Timer %15s: %0.6f\n", name, result);
#endif

    return result;
}

static void show_timing(double result, const char* name)
{
  g_print("Timer %15s: %0.6f\n", name, result);
}

#endif
