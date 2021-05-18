//A00227346
//Lizbeth Ortiz Lopez

#include <stdio.h>
#include "logger.h"
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include<signal.h> 
#define BASE46_H
#define CHUNK_SIZE_ENCODE 3
#define CHUNK_SIZE_DECODE 4

int f_size = 0;
int progress = 0;

char base46_map[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                     'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                     'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                     'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};

//Taken from: https://github.com/elzoughby/Base64/blob/master/base64.c
char* base64_encode(char* plain) {

    char counts = 0;
    char buffer[3];
    char* cipher = malloc(strlen(plain) * 4 / 3 + 4);
    int i = 0, c = 0;

    for(i = 0; plain[i] != '\0'; i++) {
        buffer[counts++] = plain[i];
        if(counts == 3) {
            cipher[c++] = base46_map[buffer[0] >> 2];
            cipher[c++] = base46_map[((buffer[0] & 0x03) << 4) + (buffer[1] >> 4)];
            cipher[c++] = base46_map[((buffer[1] & 0x0f) << 2) + (buffer[2] >> 6)];
            cipher[c++] = base46_map[buffer[2] & 0x3f];
            counts = 0;
        }
    }

    if(counts > 0) {
        cipher[c++] = base46_map[buffer[0] >> 2];
        if(counts == 1) {
            cipher[c++] = base46_map[(buffer[0] & 0x03) << 4];
            cipher[c++] = '=';
        } else {                     
            cipher[c++] = base46_map[((buffer[0] & 0x03) << 4) + (buffer[1] >> 4)];
            cipher[c++] = base46_map[(buffer[1] & 0x0f) << 2];
        }
        cipher[c++] = '=';
    }

    cipher[c] = '\0';   /* string padding character */
    return cipher;
}

//Taken from: https://github.com/elzoughby/Base64/blob/master/base64.c
char* base64_decode(char* cipher) {

    char counts = 0;
    char buffer[4];
    char* plain = malloc(strlen(cipher) * 3 / 4);
    int i = 0, p = 0;

    for(i = 0; cipher[i] != '\0'; i++) {
        char k;
        for(k = 0 ; k < 64 && base46_map[k] != cipher[i]; k++);
        buffer[counts++] = k;
        if(counts == 4) {
            plain[p++] = (buffer[0] << 2) + (buffer[1] >> 4);
            if(buffer[2] != 64)
                plain[p++] = (buffer[1] << 4) + (buffer[2] >> 2);
            if(buffer[3] != 64)
                plain[p++] = (buffer[2] << 6) + buffer[3];
            counts = 0;
        }
    }

    plain[p] = '\0';    /* string padding character */
    return plain;
}

void handle_sigint(int sig) {
    warnf("Process interruption. Ending program.\n");
    exit(1);
}

void handle_sigusr1(int sig) {
    float progress_percent = (progress * 100.0)/f_size;
    infof("Current progress: %.2f /100\n", progress_percent);
    signal(sig, handle_sigusr1);
}


int proceed(char *which, char *input_file) {

    int fd = open(input_file, O_RDONLY);
    char *output_name;

    if(strcmp(which, "encode")==0) {

        if(strcmp(input_file,"vgilante.txt")==0) output_name = "vgilante-encoded.txt";
        else if (strcmp(input_file,"sick-kid.txt")==0) output_name = "sick-kid-encoded.txt";
        else if (strcmp(input_file,"aesop11.txt")==0) output_name = "aesop11-encoded.txt";
        else if (strcmp(input_file,"megafile.txt")==0) output_name = "megafile-encoded.txt";
    } 
    else if(strcmp(which, "decode")==0) {
       if(strcmp(input_file,"vgilante-encoded.txt")==0) output_name = "vgilante-decoded.txt";
        else if (strcmp(input_file,"sick-kid-encoded.txt")==0) output_name = "sick-kid-decoded.txt";
        else if (strcmp(input_file,"aesop11-encoded.txt")==0) output_name = "aesop11-decoded.txt";
        else if (strcmp(input_file,"megafile-encoded.txt")==0) output_name = "megafile-decoded.txt";
    }
    
    f_size = lseek(fd, 0, SEEK_END);

    printf("f_size : %d \n",f_size );

    if(f_size < 0) {
        errorf("Error.\n");
        return -1;
    }

    int returning = lseek(fd, -f_size, SEEK_END);

    if(returning == -1) {
        errorf("Error.\n");
        return -1;
    }
    
    int output = open(output_name,  O_CREAT | O_WRONLY, 0644);

    if(output == -1) {
        errorf("Failed to open file.\n");
        return -1;
    }

    int chunk_size;

    if(strcmp(which, "encode") == 0) {
            chunk_size = CHUNK_SIZE_ENCODE;
        } else {
            chunk_size = CHUNK_SIZE_DECODE;
        }

    char buffer[chunk_size];
    int r;

    while(r = read(fd, buffer, chunk_size) > 0){
        char *to_do; 
        if(strcmp(which, "encode") == 0) {
            to_do = base64_encode(buffer);
        } else {
            to_do = base64_decode(buffer);
        }
        int len = strlen(to_do);
        write(output, to_do, len);
        progress += r;
    }
    infof("Done with the file\n");
}

int main(int argc, char** argv){

    if(signal(SIGINT, handle_sigint) == SIG_ERR) {
        errorf("Signal error.\n");
        return -1;
    }

    if(signal(SIGUSR1, handle_sigusr1) == SIG_ERR) {
        errorf("Signal error.\n");
        return -1;
    }


    if(argc != 3) {
        warnf("Unsupported number of parameters. Usage: ./base64 --encode/--decode <input_file>\n");
    } else {

        if(strcmp(argv[1], "--encode") == 0) {
            proceed("encode", argv[2]);
        } else if(strcmp(argv[1], "--decode") == 0) {
            proceed("decode", argv[2]);
        } else {
            warnf("Unsupported parameter. Usage: ./base64 --encode/--decode <input_file>\n");
        }
    }
    return 0;
}