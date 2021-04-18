
//a00227346
//Lizbeth Ortiz Lopez

#include <stdio.h>
#include<stdarg.h>
#include <string.h>
#include "logger.h"


#define backgroud 48 //black
#define default 37 //white

int stdout_init = 0;
int syslog_init = 0;

void setColor(int fg, int bg){
    printf("%c[0;%d;%dm",0x1B, fg, bg);
}

int initLogger(char *logType) {
    printf("Initializing Logger on: %s\n", logType);

    if(strcmp(logType, "stdout") == 0) {
        stdout_init = 1;
    } 
    else if(strcmp(logType, "syslog") == 0) {
        syslog_init = 1;
    } 
    return 0;
}

int infof(const char *format, ...) {
    setColor(35,backgroud); //information in pink
    va_list args;
    va_start(args,format);
    vprintf(format,args);
    va_end(args);
    setColor(default,backgroud);
    return 0;
}

int warnf(const char *format, ...) {
     setColor(33,backgroud); //warning in yellow
    va_list args;
    va_start(args,format);
    vprintf(format,args);
    va_end(args);
    setColor(default,backgroud);
    return 0;
}

int errorf(const char *format, ...) {
    setColor(31,backgroud); //error in red
    va_list args;
    va_start(args,format);
    vprintf(format,args);
    va_end(args);
    setColor(default,backgroud);
    return 0;
}
int panicf(const char *format, ...) {
    setColor(36,backgroud); //panic in cyan
    va_list args;
    va_start(args,format);
    vprintf(format,args);
    va_end(args);
    setColor(default,backgroud);
    return 0;
}

