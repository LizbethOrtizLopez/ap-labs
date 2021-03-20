#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define len_strings 100
void merge(char *file, char *results);

int main(int argc, char **argv)
{
    /*if (argc != 5){ //validations
        printf("Error: Invalid number of arguments \n");
        return 0;
    }
    else{ */
    printf("argv en 1 %s \n",argv[1]);
    if (strcmp(argv[1],"-n")==0){
        printf("entre \n");
        merge(argv[2],argv[4]); 
    }
    else{
        merge(argv[1],"reporte");
    }
        
    //}
    return 0;
}

void merge(char *file, char *results){

    FILE * archivo; 
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    archivo = fopen(file, "r");

    if (archivo == NULL){ //validation
       printf("Error. There is an error with the file  \n");
       exit(0);
    }

    int *arr_int;
    char **arr_string;

    size_t size_int = 4;
    size_t size_string = 4;

    arr_int = calloc(size_int,sizeof(int));
    arr_string = malloc(size_string * sizeof(char*));

    int i =0;

    while((read = getline(&line, &len, archivo)) != -1){
        i++;

        if (strcmp(file,"numbers.txt")==0){
            arr_int[i] = atoi(line);
            size_int +=1;
            arr_int = realloc(arr_int,(size_int * sizeof(int)));
        }
        else if (strcmp(file,"strings.txt")==0){
            arr_string[i] = malloc((len_strings+1)*sizeof(char));
            strcpy(arr_string[i],line);
            size_string +=1;
            arr_string = realloc(arr_string,(size_string * sizeof(char*)));
        }
        
    }

    free(arr_int);
    free(arr_string);

}
