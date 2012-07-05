//#include <stdlib.h>
#include <stdio.h>
#include <string.h>			//for strcat & strcpy
#include <glob.h>			//for glob (who would have thought)

void readDirectories(int strings_c, char* strings_v[], int do_print);
void readDirectory(char* str, int do_print);
void printError(char* error, char* directory);

int main(int argc, char* argv[]) {
	readDirectory("/usr/bin", 0);
	return 0;
}

void readDirectories(int strings_c, char* strings_v[], int do_print) {
	
	int i;
	for (i=0; i<strings_c; i++) {
		readDirectory(strings_v[i], do_print);
	}
}

void readDirectory(char* str, int do_print) {
	
	//Create directory-string
	if (str[ strlen(str)-1 ] == '/') {
		char directory[ strlen(str)+1 ];
		strcpy(directory, str);
		
		strcat(directory, "*");
	}
	else {
		char directory[ strlen(str)+2 ];
		strcpy(directory, str);
		
		strcat(directory, "/");
		strcat(directory, "*");
	}

	//printf("\n%c\n", directory[strlen(directory)] );
	
	/**
	printf("str[0]: %c\n", str[0]);
	printf("str[1]: %c\n", str[1]);
	printf("str[2]: %c\n", str[2]);
	printf("str[3]: %c\n", str[3]);
	printf("str[4]: %c\n", str[4]);
	printf("str[5]: %c\n", str[5]);
	printf("str[6]: %c\n", str[6]);
	printf("str[7]: %c\n", str[7]);
	printf("str[8]: %c\n", str[8]);
	printf("str[9]: %c\n", str[9]);
	printf("str[10]: %c\n", str[10]);
	printf("str[11]: %c\n", str[11]);
	printf("str[12]: %c\n", str[12]);
	printf("str[13]: %c\n", str[13]);
	printf("str[14]: %c\n", str[14]);
	printf("str[15]: %c\n", str[15]);
	**/
	
	
	//Check for errors
	glob_t data;
	int error = glob(str, 0, NULL, &data );
	switch(error) {
		case 0:
			printError("Everything Ok ! - OK", str);
			break;											//succeeded
		case GLOB_ABORTED:
			printError("Reading error ! - GLOB_ABORTED", str);
			return; break;
		case GLOB_NOMATCH:
			printError("No files found ! - GLOB_NOMATCH", str);
			return; break;
		case GLOB_NOSPACE:
			printError("Out of memory ! - GLOB_NOSPACE", str);
			return; break;
		default:
			printError("Something weird happend ...", str);
			return; break;
	}
	
	//Print values (if supposed to)
	if (do_print != 0) {
		int i;
		for(i=0; i<data.gl_pathc; i++) {
			//printf("File %d: %s\n", i+1, data.gl_pathv[i]);
		}
	}
	
	globfree(&data);
}

void printError(char* error, char* directory) {
	//printf(         "Error: %s\n", str);
	//fprintf(stderr, "Error: %s\n", str);
	fprintf(stderr, "\n##################################################\n");
	fprintf(stderr, "While reading directory, something went wrong !\n");
	fprintf(stderr, "Error Message: %s\n", error);
	fprintf(stderr, "Directory: %s\n", directory);
	fprintf(stderr, "##################################################\n\n");
}
