//i
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
#include <libgen.h>

struct node * hash_table[1000]; //this hash table is what we use to count how many instances of a word there are in a given file
//struct file_names * fileLL; //This is the data structure that we use to keep track of the file names sorted
struct word_nodes * word_list; //this is the data structure that we use to keep the words sorted

//This method checks if a given path is a directory or the path to a specific file
int isFile(char * path){

	struct stat s;
	if(stat(path, &s) != 0) return 0;
	return S_ISREG(s.st_mode);


}

//Our hash function
int hash(char* word){
	int hash = 0x5555555;
	
	while(*word){
		hash ^= *word++;
	}		
	return hash;
}

//our compare to method for strings
int string_compare(char* str1, char* str2){
return 1;
}

//This is a sorted insert of the words in our linked list 
void insertIntoLL(struct word_nodes* secondNode){
	if(word_list == NULL){
		word_list = (struct word_nodes*) malloc(sizeof(struct word_nodes));
		word_list->word_names = secondNode->word_names;
		word_list->next = NULL;
		return;
	}

	struct word_nodes * temp = word_list;
	struct word_nodes* prev = word_list;

	while(temp != NULL){
		if(strcmp(temp->word_names, secondNode->word_names) >= 0){
			struct word_nodes * temp2 = malloc(sizeof(struct word_nodes));	
			temp2->word_names = malloc(sizeof(char) * (strlen(secondNode->word_names) +1));
			strcpy(temp2->word_names, secondNode->word_names);
			temp2->next = temp;
			if(prev == word_list && temp != word_list->next){
				word_list = temp2;
			}else{
				prev->next = temp2;
			}
			return;
		}
		temp = temp->next;
	}

	struct word_nodes* newTemp = (struct word_nodes*) malloc(sizeof(struct word_nodes));
	newTemp->word_names = (char*) malloc(sizeof(char)* (strlen(secondNode->word_names)+1));	
	strcpy(newTemp->word_names, secondNode->word_names);
	newTemp->next = NULL;
	prev->next = newTemp;
}

//This is the insert method into out hash table and also keep tracks of the files and word count per file
void insert(char* value, char* file_name){
	int key = abs(hash(value) % 1000);
	if(hash_table[key] == NULL){
		struct node* newNode = (struct node*) malloc(sizeof(struct node));
		newNode->word = value;
		newNode->amount = 1;
		newNode->file_name = file_name;
		newNode->next = NULL;
		hash_table[key] = newNode;
		struct word_nodes* n = (struct word_nodes*) malloc(sizeof(struct word_nodes));
		n->word_names = value;
		insertIntoLL(n);
		return;
	}

	if((strcmp(hash_table[key]->file_name, file_name) != 0)){
		struct node * temp = hash_table[key];
		while(temp != NULL){
			if(strcmp(temp->file_name, file_name) == 0){
				temp->amount++;
				struct word_nodes* n = (struct word_nodes*) malloc(sizeof(struct word_nodes));
				n->word_names = value;
				insertIntoLL(n);
				return;
			}
			temp= temp->next;
		}

		struct node* nNode = (struct node*) malloc(sizeof(struct node));
		nNode->file_name = file_name;
		nNode->word = value;
		nNode->amount = 1;
		nNode->next = NULL;		
	}

	struct node* nNode = (struct node*) malloc(sizeof(struct node));
	nNode->file_name = file_name;
	nNode->word = value;
	nNode->amount = 1;
	nNode->next =hash_table[key];
	hash_table[key] = nNode;
	
	struct word_nodes* secondNode = (struct word_nodes *) malloc(sizeof(struct word_nodes));
	secondNode->word_names = value;
	insertIntoLL(secondNode);
} 

//main method
int main(int argc, char** argv){
	struct dirent *de;
	char* file_buf; 
	FILE* file, *output_file;
	if(argc!=3){
		printf("Error. Invalid number of inputs.\n");
		exit(0);
	}
	char * file_to_write_to = argv[1];
	output_file = fopen(file_to_write_to, "w");

	if(!output_file){
		perror("Error");
		exit(0);
	}
	fprintf(output_file, "<?xml version= \"1.0\" encoding=\"UTF-8\"?>\n<fileIndex>\n");
	
	char* path = argv[2];
	if(path == NULL){
		perror("Error");
		exit(0);
	}	

	DIR *dr = opendir(path);
	if(isFile(path) == 1){
		file = fopen(path, "r");
		if(file == NULL){
			perror("Error");
		}

		while(!feof(file)){
			file_buf = (char*) malloc(sizeof(char)*1000);
			fscanf(file, "%s ", file_buf);
			insert(file_buf, basename(path));
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
//			insert_into_file_list(file_name);
			file_name = de->d_name;
			full_path = (char*) malloc(sizeof(char)*strlen(file_name)*strlen(path));
			strcpy(full_path, path);
			strcat(full_path, "/");
			strcat(full_path, file_name);
			file = fopen(full_path, "r");
			if(file == NULL){
				perror("Error");
				exit(0);
			}

			while(!feof(file)){
				file_buf = (char*) malloc(sizeof(char*)*1000);
				fscanf(file, "%s ", file_buf);
				insert(file_buf, file_name);
			}
		}
	}
	//Go into the hash table and sort everything by file and word into the text file that was specified as the first argument. 
	
	struct word_nodes * temp = word_list;

	while(temp != NULL){
		if(temp->next == NULL){
			break;
		}
		int key = hash(temp->word_names)%1000;
		temp = temp->next;
		fprintf(output_file, "\t<word text=\"%s\">\n\t\t", hash_table[key]->word);
		struct node * temp2 = hash_table[key];
		while(temp2 != NULL){
			fprintf(output_file, "<file name=\"%s\">%d</file>\n\t\t", hash_table[key]->file_name, hash_table[key]->amount);
			temp2 = temp2->next;
		}
		fprintf(output_file, "</word>\n");
	}
	fprintf(output_file, "</fileIndex>");	

	closedir(dr);
    return 0;
    
}
