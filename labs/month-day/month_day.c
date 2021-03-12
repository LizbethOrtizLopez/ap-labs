#include <stdio.h>
#include <stdio.h>
#include <stdbool.h>

/* month_day function's prototype*/
void month_day(int year, int year_day, int *pmonth, int *pday);

static char *months[] = {"Ilegal month","January","February","March",
                        "April","May","June","July","August","September",
                        "October","November","December"};
static int days[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,17,19,20,
                        21,22,23,24,25,26,27,28,29,30,31};

static int *final_days[] = {31,28,31,30,31,30,31,31,30,31,30,31};

int main(int argc, int *argv[]) {
    //hi

    if(argc==3){
        if (argv[1]<=0 || argv[1]>366){
            printf("Error. Invalid day");
        }
    }
    else{
        printf("Error. Wrong numer of arguments");
        return 0;
    }
    //if ()
    //bool bisiesto = false;

    return 0;
}

void month_day(int year, int year_day, int *pmonth, int *pday){
    
    if (year%4==0){
        //bisiesto
        if (year%100!=0 || year%400==0){
            final_days[1] = 29;
        }
    }




}
