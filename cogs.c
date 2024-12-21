/*
*
*    == [ Web Router ] ==
*
* @description: A web path router used for cLib+/Websign's web server
* @author: Algo1337
* @since: 12/19/2024
*
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <str.h>
#include <arr.h>
#include <OS/utils.h>

typedef enum Object_T {
    T_ERR           = 74930
    INT_T           = 74931,
    INT_PTR_T       = 74932,
    LONG_T          = 74933,
    LONG_PTR_T      = 74934,
    CHAR_T          = 74935,
    CHAR_PTR_T      = 74936,
    CHAR_DPTR_T     = 74937,

    VOID_FN_T       = 74938,
    VOID_PTR_FN_T   = 74939,
    INT_FN_T        = 74940,
    INT_PTR_FN_T    = 74941,
    CHAR_FN_T       = 74942,
    CHAR_PTR_FN_T   = 74943,
    CHAR_DPTR_FN_T  = 74944,
    LONG_FN_T       = 74945,
    LONG_PTR_FN_T   = 74946,
    OTHER_T         = 74947
} Object_T;

typedef struct Object {
    Object_T     Type;          // Object Type
    char         *Name;         // Object Name
    void         *Data;         // Object Value
} Object;

typedef struct WebRoute {
    char        *Url;
    char        *Path;          // Path to file
    Object      **Objects;      // All objects in file
    long        ObjectCount;    // Count Of Objects Found
    char        **Flags;        // Compile flags
    void        *Handle;        // lib Handle
    struct stat  file_stat;     // File Info
    time_t       last_modified; // Last Modified for hot-reloading
} WebRoute;

typedef struct Routes {
    WebRoute **arr;
    long idx;
} Routes;

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
WebRoute *FetchLibrary(char *route_path, char *path, char **flags) {
    if(!path)
        return ((WebRoute){});

    String filepath = NewString(path);
    String c_file = NewString(fetch_c_file(path));
    WebRoute *r = (WebRoute *)malloc(sizeof(WebRoute));
    *r = (WebRoute){
        .Url = route_path,
        .Path = strdup(filepath.data),
        .Flags = flags
    };
    
    /* Parse a C file for objects/functions before compilation! */
    if(filepath.Contains(&filepath, ".c")) {
        Array lines = NewArray(NULL);
        lines.Merge(&lines, (void **)c_file.Split(&c_file, "\n"));

        for(int i = 0; i < lines.idx; i++) {
            // Find Non-Tab-Nested C-Type Starting Lines for Objects/Functions
        }

        /* Compile using flags */
        String compile_cmd = NewString("gcc -c");
        compile_cmd.AppendArray(&compile_cmd, (const char *[]){path, " ", NULL});

        if(flags)
            compile_cmd.AppendArray(&compile_cmd, flags);

        char *resp = Execute(compile_cmd);
        if(!resp) {
            printf("[ x ] Error, Unable to compile the C code....!\n");
            lines.Destruct(&lines);
            compile_cmd.Destruct(&compile_cmd);
            return ((WebRoute){});
        }

        lines.Destruct(&lines);
        compile_cmd.Destruct(&compile_cmd);
    }

    /* 
        Load .so lib!

        Retrieve functions starting with the following symbol: design_<filename>
        Or ends with the following symbol: <filename>_handler
    */
    r->Handle = dlopen(path, RTLD_LAZY);
    if(!r->Handle)
        return ((WebRoute){});

    if(filepath.Contains(&filepath, ".so"))
        for(int i = 0; i < 3; i++)
            filepath.TrimAt(&filepath, filepath.idx - 1);

            
    if(filepath.Contains(&filepath, "/"))
            filepath.TrimAt(&filepath, 0);
    
    r->Objects = (void **)malloc(sizeof(void *) * 2);

    String design_fn = NewString("design_");
    String handler_fn = NewString(filepath.data);

    design_fn.AppendString(&design_fn, filepath.data);
    handler_fn.AppendString(&handler_fn, "_handler");

    r->Objects[0] = (Object *)malloc(sizeof(Object));
    *r->Objects[0] = (Object){
        .Name = design_fn.data,
        .Type = VOID_FN_T,
        .Data = dlsym(r->Handle, design_fn.data),
    };

    if(!r->Objects[0]->Data) {
        r->Objects[0]->Type = T_ERR;
        printf("[ ~ ] Error, Trying to get the route designer function from %s....!\n", path);
    }

    r->Objects[1] = (Object *)malloc(sizeof(Object));
    *r->Objects[1] = (Object){
        .Name = handler_fn.data,
        .Type = VOID_FN_T,
        .Data = dlsym(r->Handle, handler_fn.data),
    };

    char *error = dlerror();
    if(error) {
        r->Type = T_ERR;
        dlclose(r->Handle);
        printf("[ ~ ] Error, Trying to get the route handler function from %s....!\n", path);
    }

    design_fn.Destruct(&design_fn);
    handler_fn.Destruct(&handler_fn);
    filepath.Destruct(&filepath);
    c_file.Destruct(&c_file);
    
    return r;
}

int AddWebRoute(Routes *r, WebRoute *wr) {
    if(!r || !rw)
        return 0;

    r->arr[r->idx] = (void *)malloc(sizeof(void));
    r->arr[r->idx] = wb;
    r->idx++;
    r->arr = (WebRoute **)realloc(r->arr, sizeof(WebRoute *) * (r->idx + 1));

    return 1;
}

Routes *InitRouter() {
    Routes *r = (Routes *)malloc(sizeof(Routes));
    *r = (Routes){
        .arr = (Routes **)malloc(sizeof(Routes *) * 1),
        .idx = 0  
    };

    return r;
}

void DestructRouter(Routes *r) {
    if(!r)
        return;

    for(int i = 0; i < r->idx; i++) {
        if(!r->arr[i])
            break;

        
        for(int obj = 0; obj < r->arr[i]->ObjectCount; obj++) {
            free(r->arr[i]->Objects[ob]->Name);
            free(r->arr[i]->Objects[ob]);
        }

        dlclose(r->arr[i]->Handle);
        free(r->arr[i]);
    }
}

/* A hot-reloading cogs system used as a router for web servers in C */
int main() {
    Routes *r = InitRouter();
    WebRoute *index = FetchLibrary("/", "/test.c", (const char *[]){"-lpthread", NULL});
    WebRoute *contact = FetchLibrary("/contact", "/contact.so", NULL);

    int add_chk = AddWebRoute(r, index);
    if(!add_chk)
        return 0;

    add_chk = AddWebRoute(r, contact);
    if(!add_chk)
        return 0;

    

    return 0;
}