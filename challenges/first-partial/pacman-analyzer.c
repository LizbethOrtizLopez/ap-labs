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
#define TABLE_SIZE 30


void analizeLog(char *logFile); // char *report);
//unsigned int hash(char *name);
//void init_hash_table();
//bool hash_table_insert(paquetes *p);
//paquetes *hash_table_lookup (char *name);
//void print_table();

typedef struct paquetes{
    char *name;
    char *install_date;
    char *last_update;
    int *updates;
    char *removal_date;

    struct paquetes *next;
} paquetes; 

//typedef struct node{
//    paquetes p;
//    struct node *next;
//}//node_t;

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

/*void push(node_t ** head,char *nombre, char install_date, char last_update, int updates, char removal_date){
    node_t * new_node;
    new_node = (node_t*) malloc(sizeof(node_t));

    new_node->p.name = nombre;
    new_node->p.install_date = install_date;
    new_node->p.last_update = last_update;
    new_node->p.updates = updates;
    new_node->p.removal_date = removal_date;

    //hash_table_insert(&new_node->p);
    new_node->next = *head;
    *head = new_node;
}
void print_list(node_t * head){
    node_t * current =  head;
    while(current!= NULL){
        printf("%s\n", current->p.name);
        current = current->next;
    }
}*/

int main(int argc, char **argv) {

    if (argc < 2) {
	    printf("Usage:./pacman-analizer.o \n");
	    return 1;
    }

    //printf("%s",argv[1]);

    //analizeLog(argv[1], REPORT_FILE);

    init_hash_table;

    analizeLog(argv[1]);

    //paquetes one = {.name="gcc", .install_date="2020",.last_update="2021",.updates=4,.removal_date="2022"};
    //paquetes two = {.name="lolca", .install_date="2020",.last_update="2021",.updates=4,.removal_date="2022"};
    //paquetes three = {.name="kkk", .install_date="2020",.last_update="2021",.updates=4,.removal_date="2022"};
    //paquetes four = {.name="pepe", .install_date="2020",.last_update="2021",.updates=4,.removal_date="2022"};
    //paquetes five = {.name="lola", .install_date="2020",.last_update="2021",.updates=4,.removal_date="2022"};

    //hash_table_insert(&one);
    //hash_table_insert(&two);
    //hash_table_insert(&three);
    //hash_table_insert(&four);
    //hash_table_insert(&five);

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

    char name_tmp[MAX_NAME];
    char install_date_tmp[MAX_DATE_INSTALL];
    char last_update_tmp[MAX_DATE_UPDATE];
    int updates_tmp;
    char removal_date_tmp[MAX_DATE_REMOVE];

    /*node_t * head = NULL;
    head = (node_t*)malloc(sizeof(node_t));
    paquetes pack_tmp; //={.name=" ", .install_date=" ",.last_update=" ",.updates=0,.removal_date=" "};
    head->p = pack_tmp;*/

    while((read = getline(&line, &len, archivo)) != -1 && contador<20){
        contador++;

        char *cadena[100];
        int i =0;

        char *token = strtok(line, " ");
        while(token!=NULL){
            printf("esto vale el token %s \n", token);
            cadena[i] = token;
            i++;
            token = strtok(NULL," ");
        }
        
        if(strcmp(cadena[2],"installed")==0){
            installed_packages++;

            strcpy(name_tmp,cadena[3]);
            strcpy(install_date_tmp,cadena[0]);

            //push(&head,*name_tmp,*install_date_tmp,"NONE",0,"NONE");

            paquetes pack = {.name=name_tmp, .install_date=install_date_tmp,.last_update=" ",.updates=0,.removal_date=" "};
            //paquetes tmp;
            //memcpy(&tmp,&pack,sizeof(pack));
            hash_table_insert(&pack);

            //free(tmp);
        }
        if(strcmp(cadena[2],"upgraded")==0){
            upgraded_packages++;
            hash(cadena[3]);


        }

        //memcpy(line,cadena,sizeof(line));
        //print_table();

    }

    print_table();

    // Implement your solution here.

    //printf("Report is generated at: [%s]\n", report);
}

