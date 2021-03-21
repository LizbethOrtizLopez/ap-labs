#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>


#define len_strings 100
void read(char *file, char *results);
int numcmp(char *s1, char *s2);
void mergeSort(void* datos[], int ini, int fin, int (*comparacion)(void *, void*));
void merge(void* datos[], int l, int m, int r, int (*comparacion)(void *, void*));


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
        read(argv[2],argv[4]); 
    }
    else{
        read(argv[1],"reporte");
    }
        
    //}
    return 0;
}

int numcmp(char *s1, char *s2) {
    double v1, v2;
    v1 = atof(s1);
    v2 = atof(s2);
    if(v1 < v2) 
        return -1;
    if(v1 > v2)
        return 1;
    return 0;
}

void mergeSort(void* datos[], int ini, int fin, int (*comparacion)(void *, void*))
{
    int numeric = 1;
    if(ini<fin)
    {
        int mid=(fin+ini)/2;
        mergeSort(datos,ini,mid,(*comparacion));
        mergeSort(datos,mid+1,fin,(*comparacion));
        merge(datos,ini,mid,fin,comparacion);
    }   
    else{
        return;
    } 
}

void merge(void* datos[], int l, int m, int r, int (*comparacion)(void *, void*))
{  
    printf("hiiiii \n");   
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
    void* L[n1];
    void* R[n2];

    printf("esto es la r %d",r);

    printf("estos son los datos: ");

    for (int i=0;i<r;i++){
        printf("%d ",datos[i]);
    }

    printf("\n aqui sigo \n");

    for (i = 0; i < n1; i++){
         L[i] = datos[l + i];
    }
    for (j = 0; j < n2; j++){
        R[j] = datos[m + 1+ j];
    }


    i = 0;
    j = 0;
    k = l;

    printf("aqui sigo 2 \n");
    while (i < n1 && j < n2)
    {
        printf("oa \n");
        if ((*comparacion)(L[i],R[j])<0)
        {
            datos[k] = L[i];
            i++;
        }
        else
        {
            datos[k] = R[j];
            j++;
        }
        k++;
       
    }
    while (i < n1)
    {
        datos[k] = L[i];
        i++;
        k++;
    }
    while (j < n2)
    {
        datos[k] = R[j];
        j++;
        k++;
    }    
}

void read(char *file, char *results){

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
    int numeric = 0;

    if (strcmp(file,"numbers.txt")==0){
        numeric = 1;
    }
    else{
        numeric = 0;
    }

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
    //mergeSort(arr_int,0,i-1,(int (*)(void *, void *))numeric?numcmp:strcmp);

    printf("hi \n");

    printf(" esta es la i %d \n", i);


    /*printf("estos son los datos 1: ");
    for (int i=0;i<sizeof(arr_int);i++){
        printf("%d ",arr_int[i]);
    }*/

    numeric ? mergeSort(arr_int,0,i, (int (*)(void*, void*))(numcmp)) : mergeSort(arr_int,0,i, (int (*)(void*, void*))(strcmp));


    free(arr_int);
    free(arr_string);

}

/*void imprimeArreglo(void* arr)
{
    for (E valor:arr)
    {
        System.out.print(valor+" ");
    }
    System.out.println("");
}*/
