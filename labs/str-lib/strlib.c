
//Lizbeth Ortiz Lopez
//A00227346

int mystrlen(char *str){
    int i = 0;
    char c;
    while(c=str[i]!='\0'){
        i++;
    }
    return i;
}

char *mystradd(char *origin, char *addition){
    
    char *final = origin + mystrlen(origin);

    while(*addition!='\0') {
        *final++ = *addition++;
    }
    *final = '\0';
    return origin;
}

int mystrfind(char *origin, char *substr){

    int len = mystrlen(origin);
    int len_substr = mystrlen(substr);

    int  i = 0,
         j = 0;

    int  cont = 0;

    while(i<len){
        if (origin[i]==substr[0]){
            j = 0;
            cont = i;
            while(j<len_substr){
                if (origin[cont]==substr[j]){
                    cont++;
                }
                else{
                    break;
                }
                j++;
            }
            if (j==len_substr){
                return i+1;
            }
        }
        i++;
    }
    return -1;
}
