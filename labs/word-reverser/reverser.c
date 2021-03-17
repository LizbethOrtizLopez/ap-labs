//Lizbeth Ortiz Lopez
//A00227346

#include <stdio.h>

void reverse(int lenght, char arr[]){ 
    int i;
    char tmp;
    for (i = 0; i< lenght/2;i++){ 
        tmp = arr[i];
        arr[i] = arr[lenght-i-1];
        arr[lenght-i-1] = tmp;
    }
    for (i = 0; i< lenght;i++){ 
        printf("%c",arr[i]);
        arr[i]='\0';
    }
    arr[i+1]='\0';
    printf("\n");
}

int main(){
    int i;
    char c, word[100];
    i = 0;

    while((c=getchar())!=EOF){ 
        if(c=='\n'){ 
            if (i==0) { 
                printf("Error, no escribiste nada. Intenta escribir algo\n ");
                word[i]='\0';
            }    
            else { 
                reverse(i,word);
                i=0;
            }
        }
        else { 
            word[i] = c;
            i++;
        }
    }
    return 0;
}
