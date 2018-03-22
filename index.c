//
//  index.c
//  
//
//  Created by Liz on 3/19/18.
//
//

#include "index.h"
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

struct node * hash_table[1000];
int size = 1;
int isFile(char * path){

	struct stat s;
	if(stat(path, &s) != 0) return 0;
	return S_ISREG(s.st_mode);


}

int hash(char* word){
	int hash = 0x5555555;
	
	while(*word){
		hash ^= *word++;
	//	hash = rol(hash, 5);
	}		

//	printf("The value for %s is %d\n", word, hash);
	return hash;
}

void insert(char* value){
	int key = abs(hash(value) % size);

	if(hash_table[key] == NULL){
		struct node* newNode = (struct node*) malloc(sizeof(struct node));
		newNode->word = value;
		newNode->amount = 1;
		hash_table[key] = newNode;
		size++;
	}else if(strcmp(hash_table[key]->word, value) == 0){
		hash_table[key]->amount++;
	}else{
		struct node* newNode = (struct node*) malloc(sizeof(struct node));
		newNode->word = value;
		newNode->amount = 1;
		hash_table[key] = newNode;
		size++;
	}	

	
	printf("amount for %s is: %d\n", hash_table[key]->word, hash_table[key]->amount);
} 

int main(int argc, char** argv){
    //check that argv[1] === a directory and not a file
	struct dirent *de;
	char* file_buf; 
	FILE* file;
	
	if(argc < 2){
		printf("Error. Invalid number of inputs.\n");
		exit(0);
	}
	char* path = argv[1];
	if(path == NULL){
		perror("Error");
	}	
	printf("%s\n", path);
	DIR *dr = opendir(path);

	if(isFile(path) == 1){
		file = fopen(path, "r");
		if(file == NULL){
			perror("Error");
		}

		while(!feof(file)){
			file_buf = (char*) malloc(sizeof(char)*1000);
			fscanf(file, "%s ", file_buf);
			insert(file_buf);
		}
	
		
	}else{
		char * file_name;
		char * full_path;
		while ((de = readdir(dr)) != NULL){
			if (!strcmp (de->d_name, ".")){
				continue;
			}
			if (!strcmp (de->d_name, "..")){
				continue;
			}
			file_name = de->d_name;
			printf("FILE NAME: %s\n", file_name);
			full_path = (char*) malloc(sizeof(char)*strlen(file_name)*strlen(path));
			strcpy(full_path, path);
			strcat(full_path, file_name);
			file = fopen(full_path, "r");
			if(file == NULL){
				perror("Error");
			}

			while(!feof(file)){
				file_buf = (char*) malloc(sizeof(char*)*1000);
				fscanf(file, "%s ", file_buf);
				insert(file_buf);
			}
		}
	}
    closedir(dr);
    return 0;
    
}
