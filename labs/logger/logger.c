//Lizbeth Ortiz Lopez
//A00227346

#include<stdio.h> 
#include<stdarg.h>		
#include<signal.h>

#define backgroud 48 //black

void setColor(int fg, int bg){
    printf("%c[0;%d;%dm",0x1B, fg, bg);
}

void infof(const char *format, ...){
    setColor(35,backgroud); //information in pink
    va_list args;
    va_start(args,format);
    vprintf(format,args);
    va_end(args);
}
void warnf(const char *format, ...){
    setColor(33,backgroud); //warning in yellow
    va_list args;
    va_start(args,format);
    vprintf(format,args);
    va_end(args);
}
void errorf(const char *format, ...){
    setColor(31,backgroud); //error in red
    va_list args;
    va_start(args,format);
    vprintf(format,args);
    va_end(args);
}
void panicf(const char *format, ...){ 
    setColor(36,backgroud); //panic in cyan
    va_list args;
    va_start(args,format);
    vprintf(format,args);
    va_end(args);
}