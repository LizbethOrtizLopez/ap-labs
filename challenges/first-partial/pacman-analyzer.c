#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define REPORT_FILE "packages_report.txt"
#define MAX_NAME 255
#define MAX_DATE_INSTALL 50
#define MAX_DATE_UPDATE 50
#define MAX_DATE_REMOVE 50
#define TABLE_SIZE 7


void analizeLog(char *logFile); // char *report);
//unsigned int hash(char *name);
//void init_hash_table();
//bool hash_table_insert(paquetes *p);
//paquetes *hash_table_lookup (char *name);
//void print_table();

typedef struct paquetes{
    char name[MAX_NAME];
    char install_date[MAX_DATE_INSTALL];
    char last_update[MAX_DATE_UPDATE];
    int updates;
    char removal_date[MAX_DATE_REMOVE];

    struct paquetes *next;
} paquetes; 

paquetes * hash_table[TABLE_SIZE];

unsigned int hash(char *name){
    int length = strnlen(name,MAX_NAME);
    unsigned int hash_value = 0;
    for (int i=0; i<length; i++){
        hash_value += name[i];
        hash_value = (hash_value * name[i])% TABLE_SIZE;
    } 
    return hash_value;   
} 

void init_hash_table(){
    for (int i=0; i< TABLE_SIZE; i++){
        hash_table[i] = NULL;
    }  
}   

bool hash_table_insert(paquetes *p){
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

    //printf("%s",argv[1]);

    //analizeLog(argv[1], REPORT_FILE);

    //analizeLog(argv[1]);

    init_hash_table;

    //paquetes one = {.name="gcc", .install_date="2020",.last_update="2021",.updates=4,.removal_date="2022"};
    //paquetes two = {.name="lolca", .install_date="2020",.last_update="2021",.updates=4,.removal_date="2022"};
    //paquetes three = {.name="kkk", .install_date="2020",.last_update="2021",.updates=4,.removal_date="2022"};
    //paquetes four = {.name="pepe", .install_date="2020",.last_update="2021",.updates=4,.removal_date="2022"};
    //paquetes five = {.name="lola", .install_date="2020",.last_update="2021",.updates=4,.removal_date="2022"};
    //paquetes six = {.name="chimichanga", .install_date="2020",.last_update="2021",.updates=4,.removal_date="2022"};
    //paquetes seven = {.name="kak", .install_date="2020",.last_update="2021",.updates=4,.removal_date="2022"};
    //paquetes eight = {.name="liz", .install_date="2020",.last_update="2021",.updates=4,.removal_date="2022"};

    //hash_table_insert(&one);
    //hash_table_insert(&two);
    //hash_table_insert(&three);
    //hash_table_insert(&four);
    //hash_table_insert(&five);
    //hash_table_insert(&six);
    //hash_table_insert(&seven);
    //hash_table_insert(&eight);

    //print_table();
    return 0;
}

void analizeLog(char *logFile) { 
    //char *report) {
    
    printf("Generating Report from: [%s] log file\n", logFile);

    FILE * archivo; 
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    archivo = fopen(logFile, "r");
    //if (archivo == NULL)
     //   exit();

    int contador =0;

    int installed_packages = 0;
    int removed_packages = 0;
    int upgraded_packages = 0;
    int current_installed = 0;

    while((read = getline(&line, &len, archivo)) != -1 && contador<20){
        contador++;

        char *cadena[100];
        int i =0;

        char *token = strtok(line, " ");
        while(token!=NULL){
            cadena[i] = token;
            i++;
            token = strtok(NULL," ");
        }

        if(strcmp(cadena[2],"installed")==0){
            installed_packages++;
            paquetes pack = {.name=cadena[3], .install_date=cadena[0],.last_update=cadena[0],.updates=1,.removal_date=" "};
            hash_table_insert(&pack);
        }
        if(strcmp(cadena[2],"upgraded")==0){
            upgraded_packages++;
            hash(cadena[3]);


        }


        memcpy(line,cadena,sizeof(line));

    }
    
    // Implement your solution here.

    //printf("Report is generated at: [%s]\n", report);
}

