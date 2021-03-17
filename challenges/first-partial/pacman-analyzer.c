//Log file analyzer
//Lizbeth Ortiz Lopez  A00227346
//ADVANCED PROGRAMMING 
//March, 2021

#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define REPORT_FILE "packages_report.txt"
#define MAX_NAME 1000
#define MAX_DATE 1000
#define TABLE_SIZE 200

void analizeLog(char *logFile, char *report);

//package struct with the data
typedef struct paquetes{
    char name[MAX_NAME];
    char install_date[MAX_DATE];
    char last_update[MAX_DATE];
    int updates;
    char removal_date[MAX_DATE];

    struct paquetes *next;
} paquetes; 

//hash table of packages
paquetes * hash_table[TABLE_SIZE];


//generator of key values for the packages in the hash table
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

//init the hash table
void init_hash_table(){
    for (int i=0; i< TABLE_SIZE; i++){
        hash_table[i] = NULL;
    }  
}   

//insertion of packages to the hash table
bool hash_table_insert(char name[MAX_NAME], char install_date[MAX_DATE], char last_update[MAX_DATE], int updates,char removal_date[MAX_DATE]){
    paquetes *p;
    p = (paquetes*)malloc(sizeof(paquetes));

    //insertion of data to a package
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

//looking up for an specific package
paquetes *hash_table_lookup (char *name){
    int index = hash(name);
    paquetes *tmp = hash_table[index];
    while(tmp!=NULL && (strcmp(tmp->name,name)==0)){
        tmp = tmp->next;
    }  
    return tmp;   
} 

int main(int argc, char **argv) {

    if (argc < 2) { //validations
        printf("Usage:./pacman-analizer.o \n");
        printf("Error: Invalid number of arguments \n");
        return 0;
    }
    if (argc != 5){ //validations
        printf("Error: Invalid number of arguments \n");
        return 0;
    }
    else{ 
        init_hash_table; //call to the init function
        analizeLog(argv[2],argv[4]); 
    }
    return 0;
}

//lets analize the file
void analizeLog(char *logFile, char *report) { 
    
    printf("Generating Report from: [%s] log file\n", logFile);

    //open of the file
    FILE * archivo; 
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    archivo = fopen(logFile, "r");

    if (archivo == NULL){ //validation
       printf("Error. There is an error with the file  \n");
       exit(0);
    }
       
    int contador =0;

    //counter of different packages
    int installed_packages = 0;
    int removed_packages = 0;
    int upgraded_packages = 0;
    int current_installed = 0;
    char oldest_package[MAX_NAME];
    char newest_package[MAX_NAME];

    //type counters
    int alpm_script = 0;
    int alpm = 0;
    int pacman = 0;

    //tmp variables
    char name_tmp[MAX_NAME];
    char install_date_tmp[MAX_DATE];
    char last_update_tmp[MAX_DATE];
    int  updates_tmp = 0;
    char removal_date_tmp[MAX_DATE];
    bool first = false;

    //read of lines
    while((read = getline(&line, &len, archivo)) != -1){
        contador++;

        //this part of code separates the line into an array with only the actual words of the line
        char *cadena[1000];
        int i =0;

        char *token = strtok(line, " ");
        while(token!=NULL){
            cadena[i] = token;
            i++;
            token = strtok(NULL," ");
        }//prueba

        //we can compare specific words into the array

        //new insertion in the hash table
        if((strcmp(cadena[2],"installed")==0) || (strcmp(cadena[3],"installed")==0)){
            char *tmp;
            if (strcmp(cadena[2],"installed")==0) { //pacman 1
                tmp = strcat(cadena[3],cadena[4]);
                strcpy(install_date_tmp,cadena[0]);
            }else{
                tmp = strcat(cadena[4],cadena[5]); //pacman 2
                strcpy(install_date_tmp,strcat(cadena[0],cadena[1]));
            } 

            installed_packages++;
            current_installed++;

            if (first==false){
                strcpy(oldest_package,tmp);
                first = true;
            } 

            strcpy(newest_package,tmp);
            strcpy(name_tmp,tmp);
            hash_table_insert(name_tmp,install_date_tmp,"-",0,"-");
        }
        if((strcmp(cadena[2],"upgraded")==0) || (strcmp(cadena[3],"upgraded")==0)){ //actualization of a package
            char *tmp;
            if (strcmp(cadena[2],"upgraded")==0) { //pacman 1
                tmp = strcat(cadena[3],cadena[4]);
                strcpy(last_update_tmp,cadena[0]);
            }else{
                tmp = strcat(cadena[4],cadena[5]); //pacman 2
                strcpy(last_update_tmp,strcat(cadena[0],cadena[1]));
            } 
            upgraded_packages++;
            paquetes *p = hash_table_lookup(tmp); 
            if (p!=NULL){
                strcpy(p->last_update,last_update_tmp);
                p->updates = p->updates+1;
            }
        }
        
        if((strcmp(cadena[2],"removed")==0) || (strcmp(cadena[3],"removed")==0)){ //remove of a package
            char *tmp;
            if (strcmp(cadena[2],"removed")==0) { //pacman 1
                tmp = strcat(cadena[3],cadena[4]);
                 strcpy(removal_date_tmp,cadena[0]);
            }else{
                tmp = strcat(cadena[4],cadena[5]); //pacman 2
                strcpy(removal_date_tmp,strcat(cadena[0],cadena[1]));
            }
            removed_packages++;
            current_installed--; 
            paquetes *p = hash_table_lookup(tmp); 

            if (p!=NULL){
                strcpy(p->removal_date,removal_date_tmp);
            }
        }

        //type counters 
        if ((strcmp(cadena[1],"[ALPM-SCRIPTLET]")==0) || (strcmp(cadena[2],"[ALPM-SCRIPTLET]")==0)){
            alpm_script++;
        }
        if ((strcmp(cadena[1],"[ALPM]")==0) || (strcmp(cadena[2],"[ALPM]")==0)){
            alpm++;
        }
        if ((strcmp(cadena[1],"[PACMAN]")==0) || (strcmp(cadena[2],"[PACMAN]")==0)){
            pacman++;
        }
        memcpy(line,cadena,sizeof(line));
    }

    //close of the file
    fclose(archivo);

    //creation of the new file
    FILE * reporte;

    reporte = fopen(report,"w");

    //insertions into the file
    fprintf(reporte,"Pacman Packages Report \n");
    fprintf(reporte,"---------------------- \n");
    fprintf(reporte,"-Installed Packages :  %d\n",installed_packages);
    fprintf(reporte,"-Removed Packages :  %d\n",removed_packages);
    fprintf(reporte,"-Upgraded Packages :  %d\n",(upgraded_packages-installed_packages));
    fprintf(reporte,"-Current Packages :  %d\n",current_installed);
    fprintf(reporte,"---------------------- \n");
    fprintf(reporte,"General Stats \n");
    fprintf(reporte,"---------------------- \n");
    fprintf(reporte,"-Oldest Packages :  %s",oldest_package);
    fprintf(reporte,"-Newest Packages :  %s",newest_package);
    fprintf(reporte,"-Packages with no upgrades :  ");

    for (int i=0; i< TABLE_SIZE;i++){
        if (hash_table[i] != NULL){
            paquetes *tmp = hash_table[i];
            while(tmp != NULL){
                if(tmp->updates==0){
                    fprintf(reporte,"%s",tmp->name);
                }
                tmp = tmp -> next;
            }  
        }    
    }

    fprintf(reporte,"\n");
    fprintf(reporte,"-[ALPM-SCRIPTLET] type count :  %d\n",alpm_script);
    fprintf(reporte,"-[ALPM] count :  %d\n",alpm);
    fprintf(reporte,"-[PACMAN] count :  %d\n",pacman);

    fprintf(reporte,"\nList of Packages\n");
    fprintf(reporte,"---------------------- \n");
 
    //insert all the details about the packages in the file
    for (int i=0; i< TABLE_SIZE;i++){
        if (hash_table[i] != NULL){
            paquetes *tmp = hash_table[i];
            while(tmp != NULL){
                fprintf(reporte,"Package Name       : %s",tmp->name);
                fprintf(reporte,"Install Date       : %s\n",tmp->install_date);
                fprintf(reporte,"Last Update        : %s\n",tmp->last_update);
                fprintf(reporte,"How many Updates   : %d\n",tmp->updates);
                fprintf(reporte,"Remove Date        : %s\n",tmp->removal_date);
                tmp = tmp -> next;
                fprintf(reporte,"\n");  
            }  
        }    
    } 

    fclose(reporte); //close of the report file
    printf("Report is generated at: [%s]\n",report);
}