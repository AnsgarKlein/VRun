#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glob.h>

void printError(char* error, char* directory) {
	fprintf(stderr, "\n##################################################\n");
	fprintf(stderr, "While reading directory, something went wrong !\n");
	fprintf(stderr, "Error Message: %s\n", error);
	fprintf(stderr, "Directory: %s\n", directory);
	fprintf(stderr, "##################################################\n\n");
}

int readDirectory(char* str, int doprint) {
	int str_c = strlen(str);
	char* directory;
	
	//Create directory-string
	if (str[str_c - 1] == '/') { //if str ends with '/' append '*'
		directory = malloc(str_c + 2);
		strcpy(directory, str);
		
		directory[str_c] = '*';
		directory[str_c+1] = '\0';
	}
	else { //else append '/' and '*'
		directory = malloc(str_c + 3);
		strcpy(directory, str);
		
		directory[str_c] = '/';
		directory[str_c+1] = '*';
		directory[str_c+2] = '\0';
	}
	
	
	//Check for errors
	glob_t data;
	int glob_error = glob(directory, 0, NULL, &data);
	switch(glob_error) {
		case 0: //succeeded
			break;
		case GLOB_ABORTED:
			printError("Reading error ! - GLOB_ABORTED", directory);
			break;
		case GLOB_NOMATCH:
			printError("No files found ! - GLOB_NOMATCH", directory);
			break;
		case GLOB_NOSPACE:
			printError("Out of memory ! - GLOB_NOSPACE", directory);
			break;
		default:
			printError("Something weird happend ...", directory);
			break;
	}
	if (glob_error != 0) {
		free(directory);
		globfree(&data);
		return 1;
	}
	
	//Print values (if supposed to)
	if (doprint != 0) {
		printf("\nPath  :\t-\t %s\n", directory);
		
		int i;
		for(i=0; i<data.gl_pathc; i++) {
			printf("File %d: -\t %s\n", i+1, data.gl_pathv[i]);
		}
	}
	
	free(directory);
	globfree(&data);
	return 0;
}	

int readDirectories(int directories_c, char* directories_v[], int doprint) {
	int returnValue = 0;
	
	int i;
	for (i=0; i<directories_c; i++) {
		if (readDirectory(directories_v[i], doprint) != 0) {
			returnValue = 1;
		}
	}
	
	return returnValue;
}

#ifdef WITHMAIN
int main(int argc, char* argv[]) {
	char* p[] = {"/usr/bin/", "/bin"};
	int returncode = readDirectories(2, p, 0);
	
	printf("Return-Code was: %d\n", returncode);
	return returncode;
}
#endif
