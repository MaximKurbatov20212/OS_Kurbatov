#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#define OFFSET -15

int main() {
    time_t now;
    struct tm* sp;

    time(&now);
    now += OFFSET * 60 * 60;
    sp = localtime(&now);
    
    // Another solution 
    // sp->tm_hour += OFFSET; 
    // time_t california_time = mktime(sp); // struct -> time_t
    // sp = localtime(&california_time);    // time_t -> struct

    printf("Time in California: ");
    printf("%d/%d/%02d %d:%02d %s\n",
           sp->tm_mon + 1, sp->tm_mday,
           sp->tm_year, sp->tm_hour,
           sp->tm_min, tzname[sp->tm_isdst]);

    return 0;
} 
