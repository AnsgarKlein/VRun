// coding: utf-8
// author: Niklas Rosenstein, Ansgar Klein

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glob.h>

#ifndef SCANDIR_H
#define SCANDIR_H

    #define SCANDIR_ERROR       (-1)
    #define SCANDIR_ALLOCFAIL   (-2)
    #define SCANDIR_GLOBERROR   (-3)

    /** This structure is used to create a list of strings.
        `StringNode::next == NULL` indicates end of list. **/
    typedef struct StringNode {
        char*               string;
        struct StringNode*  next;
    } StringNode;

    /** Function-type used to allocate memory on the heap. **/
    typedef void* (*AllocProc)(size_t);
    typedef void  (*FreeProc) (void* p);

    #ifdef DEBUG
        #define TOSTR(x) "" #x
        #define PRINTERROR(str) printError(str, __LINE__)

        static void printError(char* string, int line);
    #endif

    /** Ensures that the passed path ends with `/*`. **/
    static char* makeGlobby(int len, char* path, AllocProc alloc);

    /** Globs a directory and fills the passed `StringNode**` with the
        beginning of the list of paths. All `StringNode*` objects are allocated
        using the passed allocator. The passed string is ensured to end with
        `/*`. A non-zero return-value indicates an error. The `FreeProc` value
        must be passed in case an allocation failed during list-creation.
        Previously allocated memory must be freed in this case. **/
    int scanDirectory(char* path, StringNode** listhead, AllocProc alloc,
                      FreeProc free);

#endif // SCANDIR_H
