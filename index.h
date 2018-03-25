//
//  index.h
//  
//
//  Created by Liz on 3/19/18.
//
//

#ifndef index_h
#define index_h

#include <stdio.h>

struct node{
    
    int amount;
    char* word;
    char* file_name;
    struct node* next;    
};

struct file_names{
	char* name;
	struct file_names* next;
};

struct word_nodes{
	char* word_names;
	struct word_nodes* next;
};

#endif /* index_h */
