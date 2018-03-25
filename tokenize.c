#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ORIGINAL_SIZE 50

struct node {
	char * string;
	struct node *next;
};

struct node *list;

void printList(struct node * list){
	struct node * ptr = list;
	while(ptr != NULL){
		printf("%s\n", ptr->string);
		ptr = ptr->next;
	}
	free(list);
}


struct node Insert(struct node *newNode, struct node *head){
	
	/* If LL is empty, create a head and fill in w/ string*/
	if(head == NULL){
		head = (struct node *) malloc(sizeof(struct node));
		head->string = malloc(sizeof(char) * (strlen(newNode->string) + 1));
		strcpy(head->string, newNode->string);
		head->next = NULL;
		return head;
	}
	
	struct node * ptr =  head;
	struct node * prev = head;
	
	while(ptr != NULL){ 
		
		// if newNode < ptr, newNode goes before ptr
		if(strcmp(ptr->string, newNode->string) > 0){
			struct node * tmp = malloc(sizeof(struct node));
			tmp->string = malloc(sizeof(char) * (strlen(newNode->string) + 1));
			strcpy(tmp->string, newNode->string);
			tmp->next = ptr;
			
			if(prev == head && ptr != head->next){
				head = tmp;
			} else {
				prev->next = tmp;
			} return head;
			
		}
		prev = ptr;
		ptr = ptr->next;
	}
	
	struct node * tmp = malloc(sizeof(struct node));
	tmp->string = malloc(sizeof(char) * (strlen(newNode->string + 1)));
	strcpy(tmp->string, newNode->string);
	tmp->next = NULL;
	prev->next = tmp;
	return head;
}
			


int curr = 0;
//int len = 0;

int main(int argc, char **argv){
	FILE *fp = NULL;
	char *filename = NULL;
	char *str; 
	str = (char*)malloc(sizeof(char) * ORIGINAL_SIZE);
	
	printf("one byte in str: %d\n", (int)sizeof(str));
	printf("ORIGINAL_SIZE = %d\n", ORIGINAL_SIZE);
	printf("str size %d\n", (int)(sizeof(str) * ORIGINAL_SIZE));
	
	if(str == NULL){
		printf("Memory allocation failed!\n");
		exit(0);
	}
	
	if(argc < 2){
		printf("error\n");
		exit(0);
	}
	filename = argv[1];
	printf("filename is %s\n", filename);
	fp = fopen(filename, "r");
	
	if(fp == NULL){
		printf("error: file doesn't exist\n");
		exit(0);
	}
	
	
	/* reads the file and stores the entire string inside an array,
	   converting all uppercase to lowercase and changing any symbol
	   to a '-' character */
	int malloc_count = 0;   
	int counter = 0;
	while(!feof(fp)){
		fscanf(fp, "%c", &str[counter]);
		
		/* Checks to see if size of str needs to be increased for file input */
		malloc_count += 8;
		printf("malloc count:%d\n", malloc_count);
		
		//int len = strlen(str);
		//printf("len = %d\n", len);
		if(malloc_count == sizeof(str)){
			int new_size =  sizeof(str) * ORIGINAL_SIZE;
			str = realloc(str, new_size);
			continue;
		} 
		
		if(str[counter] >= 65 && str[counter] <= 90){
			str[counter] += 32;
		}
		
		if(!(isalpha(str[counter]) || (isdigit(str[counter])))){
			str[counter] = '-';
		}
		
		//printf("%c  %d\t %d\n", str[counter], str[counter], counter);
		
		counter++;
	}
	
	fclose(fp);
		
	int length = strlen(str);
	//printf("length:%d\n", length);

	//printf("%s\n", str);
	
	/* Iterates through str and creates substrings as it goes.
	   Each substring is then stored inside of a linked list.
	*/
	while(curr < length){
		int len = 0;
		
		if((str[curr] == '-') || isdigit(str[curr])){
			curr++;
			continue;
		}
		
		while( isalpha(str[curr]) || isdigit(str[curr])){
			curr++;
			len++;
		}
		
		//printf("len: %d\n", len);
		//printf("curr: %d\t%c\n", curr, str[curr]);
		//printf("str = %d\n", *(str));
		
		
		//char * word = malloc(sizeof(char) * len);
		char word[len + 1];
		strncpy(word, str + curr - len, len);
		word[len] = '\0';
		
	//	printf("%s\n", word);
		
		struct node * newNode = (struct node *) malloc(sizeof(struct node));
		newNode->string = malloc(sizeof(char) * (strlen(word) + 1));
		strcpy(newNode->string, word);
		
		list = Insert(newNode, list);
		

		len = 0;

		//printf("%d\n", curr);	
				
		//printf("%s\n", str);
		//curr++;
	}
	
	printList(list);
	free(str);	
	
	
	return 0;
}
