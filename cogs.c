#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <str.h>
#include <arr.h>
#include <OS/utils.h>

typedef enum Object_T {
    NO_T            = 74930
    INT_T           = 74931,
    INT_PTR_T       = 74932,
    LONG_T          = 74933,
    LONG_PTR_T      = 74934,
    CHAR_T          = 74935,
    CHAR_PTR_T      = 74936,
    CHAR_DPTR_T     = 74937,

    INT_FN_T        = 74938,
    INT_PTR_FN_T    = 74939,
    CHAR_FN_T       = 74940,
    CHAR_PTR_FN_T   = 74941,
    CHAR_DPTR_FN_T  = 74942,
    LONG_FN_T       = 74943,
    LONG_PTR_FN_T   = 74944,
    OTHER_T         = 74945
} Object_T;

typedef struct Object {
    Object_T     Type;          // Object Type
    char         *Name;         // Object Name
    void         *Data;         // Object Value
} Object;

typedef struct WebRoute {
    char        *Path;          // Path to file
    Object      **Objects;      // All objects in file
    char        **Flags;        // Compile flags
    void        *Handle;        // lib Handle
    struct stat  file_stat;     // File Info
    time_t       last_modified; // Last Modified for hot-reloading
} WebRoute;

typedef struct Routes {
    WebRoute **arr;
    long idx;
}

char *fetch_c_file(char *filepath) {
    if(!filepath)
        return NULL;

    FILE *c_file = fopen(filepath, "r");
    if(!c_file)
        return NULL;

    fseek(c_file, 0L, SEEK_END);
    long sz = ftell(c_file);
    fseek(c_file, 0L, SEEK_SET);

    char *data = (char *)malloc(sz);
    if(!data)
        return NULL;

    memset(data, '\0', sz);
    fread(data, sz, 1, c_file);
    return data;
}

/*
    Provide a library filepath using .c or .so files
*/
WebRoute *FetchLibrary(char *path, char **flags) {
    if(!path)
        return ((WebRoute){});

    WebRoute *r = (WebRoute *)malloc(sizeof(WebRoute));
    *r = (WebRoute){
        .Path = path,
        .Flags = flags
    };

    String filepath = NewString(path);
    String c_file = NewString(fetch_c_file(path));
    
    /* Parse a C files for objects/function before compilation! */
    if(filepath.Contains(&filepath, ".c")) {
        Array lines = NewArray(NULL);
        lines.Merge(&lines, (void **)c_file.Split(&c_file, "\n"));

        for(int i = 0; i < lines.idx; i++) {
            // Find Non-Nested C-Type Starting Lines for Objects/Functions
        }

        /* Compile using flags */
        String compile_cmd = NewString("gcc -c");
        compile_cmd.AppendArray(&compile_cmd, (const char *[]){path, " ", NULL});
        compile_cmd.AppendArray(&compile_cmd, flags);

        char *resp = Execute(compile_cmd);
        if(!resp) {
            printf("[ x ] Error, Unable to compile the C code....!\n");
            return ((WebRoute){});
        }
    }

    /* 
        Retrieve functions starting with the following symbol: design_<filename>
        Or ends with the following symbol: <filename>_handler
    */
}

Routes *InitRouter() {
    Routes *r = (Routes *)malloc(sizeof(Routes));
    *r = (Routes){
        .arr = (Routes **)malloc(sizeof(Routes *) * 1),
        .idx = 0  
    };

    return r;
}

/* A hot-reloading cogs system used as a router for web servers in C */
int main() {
    Routes *r = InitRouter();
    WebRoute *index = FetchLibrary("/test.c", (const char *[]){"-lpthread", "-lwebsign", "-lstr", "-larr", "-lmap", NULL});



    return 0;
}