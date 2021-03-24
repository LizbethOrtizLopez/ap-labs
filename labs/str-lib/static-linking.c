//Lizbeth Ortiz Lopez
//A00227346

#include <stdio.h>
#include <string.h>

int mystrlen(char *);
char *mystradd(char *, char *);
int mystrfind(char *, char *);

int main(int argc, char *argv[]) {
    if (argc==4 && strcmp(argv[1],"-find")==0){
         int buscar = mystrfind(argv[2], argv[3]);
        if (buscar > -1) {
            printf("['%s'] string was found at [%d] position\n", argv[3], buscar);
        } else {
            printf("The input was not found \n");
        }
        return 0;  
    }
    else if (argc==4 && strcmp(argv[1],"-add")==0){
        printf("Initial Lenght      : %d\n", mystrlen(argv[2]));
        char *added_word = mystradd(argv[2],argv[3]);
        printf("New String          : %s\n", added_word);
        printf("New length          : %d\n", mystrlen(added_word));
        return 0;
    }
    else if (argc==3 && strcmp(argv[1],"-add")==0){
        printf("Error. There is no string to add. Check the parameters. \n");
    }
    else if (argc==3 && strcmp(argv[1],"-find")==0){
        printf("Error. There is no string to find. Check the parameters. \n");
    }
    else {
        printf("Error. The input or the parameters where wrong \n");
    }
    return 0;
}
