#include <stdio.h>
#include <stdlib.h>


/* month_day function's prototype*/
int month_day(int year, int year_day);

static char *months[12] = {"Jan","Feb","Mar","Apr","May","June","July","Aug","Sep",
                    "Oct","Nov","Dec"};

static int final_days[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};

int main(int argc, char *argv[]) {

    if(argc==3){
        if (atoi(argv[2])<=0 || atoi(argv[2])>366){
            printf("Error. Invalid day. \n");
            return 0;
        }
        month_day(atoi(argv[1]),atoi(argv[2]));
    }
    else{
        printf("Error. Wrong number of arguments. \n");
        return 0;
    }
    return 0;
}
int month_day(int year, int year_day){
    
    if (year%4==0)
    {
        if (year%100!=0 || year%400==0)
        {
            final_days[2] = 29; //es bisiesto
        }
        else
        {
            if (year_day==366){ //no es bisiesto
                printf("Error. Invalid day");
                return 0;
            }
        }
    }
    int acum_days = year_day;
    int i = -1;

    while(acum_days>31)
    {
        i++;
        acum_days -= final_days[i];
    }
    printf("%s %d, %d \n",months[i],acum_days,year);
    return 0;
}