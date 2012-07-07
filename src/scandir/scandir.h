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

    /** Globs the passed directory and creates a list of `StringNode*`. The
        passed *\*\*listhead* and *\*\*listtail* are filled with the head and
        tail of the resulting list. All data that is left to the caller is
        allocated via the passed `AllocProc`*alloc* function. The `FreeProc`
        argument is necessary in case an allocation failed during the process
        of creating a list.
        A non-zero return-value indicates an error. **/
    int scanDirectory(char* path, StringNode** listhead, StringNode** listtail,
                      AllocProc alloc, FreeProc free);

#endif // SCANDIR_H
