// coding: utf-8
// author: Niklas Rosenstein, Ansgar Klein

#include "scandir.h"

#ifdef DEBUG
        static void printError(char* string, int line) {
            fprintf(stderr, "%s, line %d: %s\n", __FILE__, line, string);
        }
#endif

static char* makeGlobby(int len, char* path, AllocProc alloc) {
    char* globby;
    if (path[len - 1] == '/') {
        globby = alloc(len + 2);
        if (!globby) return NULL;
        memcpy(globby, path, len);
        globby[len] = '*';
        globby[len + 1] = '\0';
    }
    else {
        globby = alloc(len + 3);
        if (!globby) return NULL;
        memcpy(globby, path, len);
        globby[len] = '/';
        globby[len + 1] = '*';
        globby[len + 2] = '\0';
    }
    return globby;
}

int scanDirectory(char* path, StringNode** listhead, AllocProc alloc,
                  FreeProc free) {
    path = makeGlobby(strlen(path), path, malloc);
    glob_t data;
    int glob_error = glob(path, 0, NULL, &data);

    #ifdef DEBUG
        switch (glob_error) {
            case 0:
                // No error.
                break;
            case GLOB_ABORTED:
                PRINTERROR("scanDirectory(): " TOSTR(GLOB_ABORTED));
                break;
            case GLOB_NOMATCH:
                PRINTERROR("scanDirectory(): " TOSTR(GLOB_NOMATCH));
                break;
            case GLOB_NOSPACE:
                PRINTERROR("scanDirectory(): " TOSTR(GLOB_NOSPACE));
                break;
            default:
                PRINTERROR("scanDirectory(): unknown non-zero return.");
                break;
        }
    #endif

    if (glob_error != 0) {
        // previously overwritten with the return-value of makeGlobby, so
        // NO, we don't free any data the user passed, here.
        free(path);
        globfree(&data);
        *listhead = NULL;
        return SCANDIR_GLOBERROR;
    }

    // Convert the globbed information to a StringNode list.
    *listhead = malloc(sizeof(StringNode));
    if (!*listhead) {
        free(path);
        globfree(&data);
        return SCANDIR_ALLOCFAIL;
    }
    StringNode* node = *listhead;
    int i;
    char* glob_path;
    int   glob_path_len;
    for (i=0; i < data.gl_pathc; i++) {
        glob_path = data.gl_pathv[i];
        glob_path_len = strlen(glob_path);
        node->string = malloc(glob_path_len + 1);
        if (!node->string) {

            #ifdef DEBUG
                PRINTERROR("scanDirectory(): Error allocating string.");
            #endif

            goto freenodes;
        }
        memcpy(node->string, glob_path, glob_path_len + 1);

        if (i < data.gl_pathc - 1) {
            node->next = alloc(sizeof(StringNode));
            if (!node->next) {

                #ifdef DEBUG
                    PRINTERROR("scanDirectory(): Error allocating StringNode*.");
                #endif

                free(node->string);
                goto freenodes;
            }
            node = node->next;
        }
    }
    node->next = NULL;

    free(path);
    globfree(&data);
    return 0;

  freenodes:
    free(path);
    globfree(&data);
    // Free previously allocated memory from the StringNode list.
    StringNode* next;
    node = *listhead;
    while (node) {
        next = node->next;
        // Do NOT free the last node's string. It either was never allocated
        // successfully or was already freed.
        if (next) {
            free(node->string);
        }
        free(node);
        node = next;
    }
    return SCANDIR_ALLOCFAIL;
}


#ifdef SCANDIR_MAIN
    int main() {
        printf("\n---------------------\n");
        char* path = "/usr/bin/";
        StringNode* node = NULL;
        int success = scanDirectory(path, &node, malloc, free);
        if (success != 0) {
            printf("scanDirectory() returned non-zero. Break.\n");
            return success;
        }

        // Free the StringNodes.
        int i = 0;
        StringNode* next;
        while (node) {
            printf("%s\n", node->string);
            next = node->next;
            free(node->string);
            free(node);
            node = next;
            i++;
        }

        return 0;
    }
#endif // SCANDIR_MAIN



