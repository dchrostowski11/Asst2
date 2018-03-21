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

int isFile(char * path){

	struct stat s;

	if(stat(path, &s) != 0) return 0;
	return S_ISREG(s.st_mode);


}
int main(int argc, char** argv){
    //check that argv[1] === a directory and not a file
    struct dirent *de;
    FILE* file;
    DIR *dr = opendir("/ilab/users/lvc31/Systems_Programming/Asst2/Example");
    
    while ((de = readdir(dr)) != NULL){
        
        if (!strcmp (de->d_name, ".")){
            continue;
        }
        if (!strcmp (de->d_name, "..")){
            continue;
        }

	if(isFile("/ilab/users/lvc31/Systems_Programming/Asst2/Example") == 1){
		file = fopen("/ilab/users/lvc31/Systems_Programming/Asst2/Example/text.txt", "r");

		if(file == NULL){
			perror("Error");
		} 	
	}

//        file = fopen("/ilab/users/lvc31/Systems_Programming/Asst2/Example/text.txt", "r");
        
/*	if(file == NULL){
            printf("%s\n", de->d_name);
            perror("Error");
        }else{
            printf("Hooray!\n");
        }*/
        
        
    }
    closedir(dr);
    return 0;
    
}
