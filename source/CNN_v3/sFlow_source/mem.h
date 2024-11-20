/***********************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
************************************************************/

#ifndef __MEM_H
#define __MEM_H

#include <stdio.h>

#define LEAKCHK 0  /* 0:no_debug   1: Memory Leak Detection */	
#define MEM_DEBUG 0 /* 0:no_debug   1:root   2:par   3:chi  */

/***** header file for mem.c *****/

size_t Mem_setmax(size_t mem);
size_t Mem_getrest(void);
size_t Mem_getused(void);
size_t Mem_getusemax(void);
size_t Mem_use(size_t mem);
size_t Mem_unuse(size_t mem);

#define Calloc(x, y) __Calloc(x, y, __FILE__, __LINE__)
void * __Calloc(size_t nmemb, size_t size, char const *fname, int line);

#define Realloc(a, b, c, d)  __Realloc(a, b, c, d, __FILE__ , __LINE__ )
void * __Realloc(void *ptr, size_t nmemb, size_t oldsize,
                 size_t newsize, char const *fname, int line);

#define Free(x, y) __Free(x, y, __FILE__, __LINE__);
void __Free(size_t  memsize, void* x, char const *fname, int line);

int Mem_print(FILE *fp);

#endif
/* __MEM_H */





