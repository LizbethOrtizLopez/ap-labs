
//Lizbeth Ortiz Lopez
//A00227346
//MERGE SORT WITH VOID POINTERS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>


#define len_strings 100
#define MAX_LINES 5000
void read(char *file, char *results);
int numcmp(char *s1, char *s2);
void mergeSort(void* datos[], int ini, int fin, int (*comparacion)(void *, void*));
void merge(void* datos[], int ini, int fin, int (*comparacion)(void *, void*));
void *datos[MAX_LINES];
int elements;


int main(int argc, char **argv)
{
    if (argc < 2){ //validations
        printf("Error: Invalid number of arguments \n");
        return 0;
    }
    if (strcmp(argv[1],"-n")==0){
        read(argv[2],"sorted_numbers.txt");
    }
    else{
        read(argv[1],"sorted_string.txt");
    }
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

void mergeSort(void* data[], int ini, int fin, int (*comparacion)(void *, void*))
{
    if(ini-fin)
    {
        int mid=(fin+ini)/2;
        mergeSort(data,ini,mid,comparacion);
        mergeSort(data,mid+1,fin,comparacion);
        merge(data,ini,fin,comparacion);
    }   
    return;

}

void merge(void* data[], int ini, int fin, int (*comparacion)(void *, void*))
{  
    int mid=((ini+fin)/2)+1;      
    for (int i=mid;i<=fin;i++)
    {
        for(int j=0;j<mid;j++)
        {
            if ((*comparacion)(data[i],data[j])<0) {
                void* tmp=datos[i];
                datos[i]=datos[j]; 
                datos[j]=tmp;
                datos[j]=data[j]; 
            }
        }
    }
}

void read(char *file, char *results){

    FILE * archivo; 
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    archivo = fopen(file, "r");

    if (archivo == NULL){ //validation
       printf("Error. There is an error with the file or the parameters are wrong  \n");
       exit(0);
    }

    int i =-1;
    int numeric = 0;

    if (strcmp(file,"numbers.txt")==0){
        numeric = 1;
    }
 
    while((read = getline(&line, &len, archivo)) != -1){
        i++;
        elements++;
        datos[i] = malloc((len_strings+1)*sizeof(char));
        strcpy(datos[i],line);
    }
    close(archivo);
    numeric ? mergeSort(datos,0,i, (int (*)(void*, void*))(numcmp)) : mergeSort(datos,0,i, (int (*)(void*, void*))(strcmp));
    imprimeArreglo(results);
}

void imprimeArreglo(char *results)
{
    FILE * reporte;
    reporte = fopen(results,"w");

    for (int i=0;i<elements;i++){
         fprintf(reporte,"%s",(char*)datos[i]);
    }

    fclose(reporte);
    printf("Report is generated at: [%s]\n",results);
}
