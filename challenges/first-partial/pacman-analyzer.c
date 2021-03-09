#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define REPORT_FILE "packages_report.txt"
#define MAX_NAME 1000
#define MAX_DATE 1000
#define TABLE_SIZE 200


//void analizeLog(char *logFile, char *report); 
void analizeLog(char *logFile);
//unsigned int hash(char *name);
//void init_hash_table();
//bool hash_table_insert(paquetes *p);
//paquetes *hash_table_lookup (char *name);
//void print_table();

typedef struct paquetes{
    char name[MAX_NAME];
    char install_date[MAX_DATE];
    char last_update[MAX_DATE];
    int updates;
    char removal_date[MAX_DATE];

    struct paquetes *next;
} paquetes; 

paquetes * hash_table[TABLE_SIZE];

unsigned int hash(char *name){
    int length = strnlen(name,MAX_NAME);
    unsigned int hash_value = 0;
    for (int i=0; i<length; i++){
        hash_value = hash_value + name[i];
        hash_value = (hash_value * name[i]);
    } 
    hash_value= hash_value % TABLE_SIZE;
    return hash_value;   
} 

void init_hash_table(){
    for (int i=0; i< TABLE_SIZE; i++){
        hash_table[i] = NULL;
    }  
}   

bool hash_table_insert(char name[MAX_NAME], char install_date[MAX_DATE], char last_update[MAX_DATE], int updates,char removal_date[MAX_DATE]){
    paquetes *p;
    p = (paquetes*)malloc(sizeof(paquetes));

    strcpy(p->name,name);
    strcpy(p->install_date,install_date);
    strcpy(p->last_update,last_update);
    p->updates = updates;
    strcpy(p->removal_date,removal_date);

    if (p == NULL) return false;
    int index = hash(p->name);
    p->next = hash_table[index];
    hash_table[index] = p;
    return true;
}

paquetes *hash_table_lookup (char *name){
    int index = hash(name);
    paquetes *tmp = hash_table[index];
    while(tmp != NULL && strncmp(tmp->name, name, MAX_NAME)!=0){
        tmp = tmp->next;
    }  
    return tmp;   
} 

void print_table(){
    for (int i=0; i< TABLE_SIZE;i++){
        if (hash_table[i] == NULL){
            printf("\t%i\t---\n",i);
        } else {
            printf("\t%i\t ",i);
            paquetes *tmp = hash_table[i];
            while(tmp != NULL){
                printf("%s - ",tmp->name);
                tmp = tmp -> next;
            }
            printf("\n");    
        }    
    }  
} 

int main(int argc, char **argv) {

    if (argc < 2) {
	    printf("Usage:./pacman-analizer.o \n");
	    return 1;
    }

    init_hash_table;

    //analizeLog(argv[2],argv[4]);

    analizeLog(argv[1]);

    return 0;
}

void analizeLog(char *logFile){ 
// char *report) { 
    
    printf("Generating Report from: [%s] log file\n", logFile);

    FILE * archivo; 
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    archivo = fopen(logFile, "r");
    if (archivo == NULL){
        exit(1);
    }
       
    int contador =0;

    int installed_packages = 0;
    int removed_packages = 0;
    int upgraded_packages = 0;
    int current_installed = 0;

    int alpm_script = 0;
    int alpm = 0;
    int pacman = 0;

    char name_tmp[MAX_NAME];
    char install_date_tmp[MAX_DATE];
    char last_update_tmp[MAX_DATE];
    int updates_tmp;
    char removal_date_tmp[MAX_DATE];

    char *oldest_package;
    char *newest_package;

    while((read = getline(&line, &len, archivo)) != -1 && contador<100){
        contador++;
        printf("hola en contador %d \n",contador);

        char *cadena[1000];
        char *tmp;
        int i =0;

        char *token = strtok(line, " ");
        while(token!=NULL){
            cadena[i] = token;
            i++;
            token = strtok(NULL," ");
        }

        if((strcmp(cadena[2],"installed")==0) || (strcmp(cadena[3],"installed")==0)){

            if (strcmp(cadena[2],"installed")==0) {
                tmp = strcat(cadena[3],cadena[4]);
            }else{
                tmp = strcat(cadena[4],cadena[5]);
            } 

            printf("hi");

            installed_packages++;
            current_installed++;
            if (contador==1) strcpy(oldest_package,tmp);
            strcpy(newest_package,tmp);
            strcpy(name_tmp,tmp);
            strcpy(install_date_tmp,cadena[0]);
            hash_table_insert(name_tmp,install_date_tmp,"-",0,"-");
        }
        /*if((strcmp(cadena[2],"upgraded")==0) || (strcmp(cadena[3],"upgraded")==0)){
            printf("entre");
            char *tmp;
            if (strcmp(cadena[2],"upgraded")==0) {
                tmp = strcat(cadena[3],cadena[4]);
            }else{
                tmp = strcat(cadena[4],cadena[5]);
            } 
            upgraded_packages++;
            strcpy(last_update_tmp,cadena[0]);
            paquetes *p = hash_table_lookup(tmp); 
            strcpy(p->last_update,last_update_tmp);
            p->updates = p->updates+1;
        }*/
        
        if((strcmp(cadena[2],"removed")==0) || (strcmp(cadena[3],"removed")==0)){
            if (strcmp(cadena[2],"removed")==0) {
                tmp = strcat(cadena[3],cadena[4]);
            }else{
                tmp = strcat(cadena[4],cadena[5]);
            }

            removed_packages++;
            current_installed--; 
            strcpy(removal_date_tmp,cadena[0]);
            paquetes *p = hash_table_lookup(tmp); 
            strcpy(p->removal_date,removal_date_tmp);
            printf("removi el valor %s",tmp);
        }

        if (strcmp(cadena[1],"[ALPM-SCRIPTLET]")==0 || strcmp(cadena[2],"[ALPM-SCRIPTLET]")==0){
            alpm_script++;
        }
        if (strcmp(cadena[1],"[ALPM]")==0 || strcmp(cadena[1],"[ALPM]")==0){
            alpm++;
        }
        if (strcmp(cadena[1],"[PACMAN]")==0 || strcmp(cadena[1],"[PACMAN]")==0 ){
            pacman++;
        }
        memcpy(line,cadena,sizeof(line));
    }
    print_table();
    fclose(archivo);

    /*FILE * reporte;

    reporte = fopen(REPORT_FILE,"w");

    fprintf(reporte,"Pacman Packages Report \n");
    fprintf(reporte,"---------------------- \n");
    fprintf(reporte,"-Installed Packages :  %d\n",installed_packages);
    fprintf(reporte,"-Removed Packages :  %d\n",removed_packages);
    fprintf(reporte,"-Upgraded Packages :  %d\n",upgraded_packages);
    fprintf(reporte,"-Current Packages :  %d\n",current_installed);
    fprintf(reporte,"---------------------- \n");
    fprintf(reporte,"General Stats \n");
    fprintf(reporte,"---------------------- \n");
    fprintf(reporte,"-Oldest Packages :  %s\n",oldest_package);
    fprintf(reporte,"-Newest Packages :  %s\n",newest_package);
    fprintf(reporte,"-Packages with no upgrades :  \n");
    fprintf(reporte,"-[ALPM-SCRIPTLET] type count :  %d\n",alpm_script);
    fprintf(reporte,"-[ALPM] count :  %d\n",alpm);
    fprintf(reporte,"-[PACMAN] count :  %d\n",pacman);

    fclose(reporte);

    printf("Report is generated at: [%s]\n",REPORT_FILE);*/
}

