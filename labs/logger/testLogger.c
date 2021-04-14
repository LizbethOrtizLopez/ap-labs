//Lizbeth Ortiz Lopez.
//A00227346

#include <stdio.h>
#include<stdlib.h>

int infof(const char *format, ...);
int warnf(const char *format, ...);
int errorf(const char *format, ...);
int panicf(const char *format, ...);

int main() {
    char *info = "Lizbeth Ortiz";
    char *warning = "Warning, there is a pandemic";
    char *error = "This code is wrong";
    char *panic = "Delete this or it will explode";

    infof("Information: %s\n",info);
    warnf("Warning: %s\n",warning);
    errorf("Error: %s\n",error);
    panicf("Panic: %s\n",panic);

    return 0;
}
