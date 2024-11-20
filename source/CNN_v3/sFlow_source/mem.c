/***********************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "mem.h"
#include "stdfunc.h"

#define MAX_MEM_LINE 999999

static int memcheck=0;
static size_t memmax = 0;
static size_t memuse = 0;
static size_t memuse_max = 0;

#if LEAKCHK
typedef struct {
    char  *filename;
    void  *add;
    int   line;
}MemAlloc ;

int length;
MemAlloc Mem[MAX_MEM_LINE];//works for 5000 DOF only
#endif


/***** Functions for memory allocation  *****/

size_t Mem_setmax(size_t mem)
{
/*+ Function: Set maximum available memory size +*/
#if LEAKCHK
int i;
for(i=0;i<MAX_MEM_LINE;i++)  {
  Mem[i].line = -1;
  Mem[i].add = NULL;
  Mem[i].filename="Not set!";
}
Rfprintf(stderr,"Mem_Leak Dectction initialized! The Maxium memory usage is %d bytes[%d GB].\n", mem, mem/(1024*1024));
length = 0;
#endif
        memcheck = 1;
        memmax = mem;
        return (memmax);
}

size_t Mem_getrest(void)
{
/*+ Function: Return available memory (maxmem - used)  +*/
        if (memcheck ==0) {
                Pfprintf(stderr, "Mem_getrest: Warning!!, "
                       "Call Mem_setmax before calling this function\n");
           return(0);
        } else if (memuse > memmax ) {
                Pfprintf(stderr, "Mem_getrest: Warning!!, "
                       "Already exceed mammax!!!\n");
                Pfprintf(stderr, "Mem_getrest: Warning!!, "
                       "Memuse %lu, Memmax %lu\n", (unsigned long) memuse, (unsigned long) memmax);
           return(0);
        }
        return (memmax-memuse);
}

size_t Mem_getused(void)
{
/*+ Function: Return allocated  memory size  +*/

#if LEAKCHK
int i;
for (i=0;i<MAX_MEM_LINE;i++){
  if (Mem[i].line!= -1){
    Pfprintf(stdout,"%s line %d is not freeed! Add: %ld\n",Mem[i].filename, Mem[i].line, Mem[i].add);
    }
}
#endif
        return (memuse);
}

size_t Mem_getusemax(void)
{
/*+ Function: Return maxmum memory size which previously set +*/
        return (memuse_max);
}

size_t Mem_use(size_t mem)
{
/*+ Function: Set more "mem" is allocated  +*/
        size_t memuse_new ;

        memuse_new = memuse + mem;

        if (memuse_new < memuse ) {
                Pfprintf(stderr, "Mem_use: Warning!!, "
                       "Memory size seems exceeded size_t!!!\n");
                return(0);
        } 

        if ( (memcheck == 1 ) && (memuse_new > memmax ) ){
                Pfprintf(stderr, "Mem_use: Warning!!, "
                       "Memory size exceeded memmax %lu!!!\n", (unsigned long) memmax);
                return(0);
        }

            memuse = memuse_new;

        if (memuse_max < memuse ) memuse_max = memuse;

        return(mem);
}

size_t Mem_unuse(size_t mem)
{
/*+ Function: Set "mem" is freed  +*/


        if ( memuse < mem ) {
                Pfprintf(stderr, "Mem_unuse: Warning!!, "
                       "Memory already seems to be too much freed!!!\n");
                return(0);
        }

        memuse -= mem;

        return(mem);

}


void * __Calloc(size_t nmemb, size_t size, char const *fname, int line)
{
/*+ Function: calloc with remembering allocated size  +*/

        size_t usize ;
        void *ret;

        usize = size * nmemb;
        
        if (usize  == 0 ) {
                return( NULL );
        }

        if ( Mem_use(usize ) !=  usize ) {
                Pfprintf(stderr, 
                  "Calloc: Memory allocation failed in %s at line %d\n",
                         fname, line);
                Pfprintf(stderr, 
                  "Currentry %lu bytes are used, %lu bytes were requested, "
                  "and maxmem was set to %lu bytes.\n",
                  (unsigned long) memuse, (unsigned long) usize, 
                  (unsigned long) memmax);
                Abort(-1);
        }
#if( MEM_DEBUG >= 1 )
#if ( MEM_DEBUG == 1 ) 
        if ( am_I_root_proc() ) {

#elif ( MEM_DEBUG == 2 ) 
        if ( am_I_parent() ) {

#elif ( MEM_DEBUG == 3 )
       if ( am_I_child()) {
#endif
         Pfprintf(stderr, "Calloc %lu bytes: in %s at line %d, ", 
                        (unsigned long) nmemb*size, fname, line);
         Mem_print(stderr);
       }

#endif
	
        ret = calloc(nmemb,size);

#if LEAKCHK	
    length++;
	Mem[length].filename = fname;
    Mem[length].add = ret;
	Mem[length].line = line;
#endif	
        if ( ret == NULL ) {
                Pfprintf(stderr, 
                  "Calloc: Memory allocation failed in %s at line %d\n", 
                  fname, line);
                Pfprintf(stderr, 
                   "Currentry %lu bytes are used, %lu bytes were requested.\n", 
                   (unsigned long) memuse-usize, (unsigned long) usize);

                 Abort(-1);

        }

        return( ret ) ;
}




void * __Realloc(void *ptr, size_t nmemb, size_t oldsize, 
                 size_t newsize, char const *fname, int line)
{
/*+ Function: realloc with remembering allocated size  +*/

        size_t usize_old ;
        size_t usize_new ;
        void *ret;

        usize_old = oldsize*nmemb;
        usize_new = newsize*nmemb;

        if ( Mem_unuse(usize_old ) !=  usize_old ) {
                Pfprintf(stderr, 
                   "Realloc: Memory allocation failed in %s at line %d\n" ,
                   fname, line);
                Pfprintf(stderr, 
                   "Currentry %lu bytes are used, "
                   "reallocation from %lu to %ld bytes were requested, "
                   "and maxmem was set to %lu bytes.\n",
                    (unsigned long) memuse, (unsigned long) usize_old, 
                   (unsigned long) usize_new, (unsigned long) memmax);
                Abort(-1);
        }


        if ( Mem_use(usize_new ) !=  usize_new ) {
                Pfprintf(stderr, 
                   "Realloc: Memory allocation failed in %s at line %d\n" ,
                   fname, line);
                Pfprintf(stderr, 
                   "Currentry %lu bytes are used, "
                   "reallocation from %lu to %ld bytes were requested, "
                   "and maxmem was set to %lu bytes.\n",
                    (unsigned long) memuse, (unsigned long) usize_old, 
                   (unsigned long) usize_new, (unsigned long) memmax);
                Abort(-1);
        }

#if( MEM_DEBUG >= 1 )
#if ( MEM_DEBUG == 1 ) 
        if ( am_I_root_proc() ) {

#elif ( MEM_DEBUG == 2 ) 
        if ( am_I_parent() ) {

#elif ( MEM_DEBUG == 3 )
       if ( am_I_child()) {
#endif
       Pfprintf(stderr, "Realloc: from %lu to %lu in %s at line %d\n", 
             (unsigned long) usize_old, (unsigned long) usize_new, fname, line);
       Mem_print(stderr);
       }

#endif

        if ( ( usize_old == 0 ) || ( ptr == NULL ) ) {
           if ( usize_new == 0 ) {
             return( NULL ) ;
           } else {
                ret = calloc(nmemb, newsize);
                if ( ret == NULL ) {
                Pfprintf(stderr, 
                   "Realloc: Memory allocation failed in %s at line %d\n" ,
                   fname, line);
                Pfprintf(stderr, 
                   "Currentry %lu bytes are used, "
                   "reallocation from %lu to %ld bytes were requested, "
                   "and maxmem was set to %lu bytes.\n",
                    (unsigned long) (memuse-usize_new +usize_old) , 
                    (unsigned long) usize_old, 
                   (unsigned long) usize_new, (unsigned long) memmax);

                 Abort(-1);

                 }

             return( ret ) ;
           }
        }

        if ( usize_new ==  0 ) {
             free( ptr ) ;
             return(NULL);
        }

        ret =  realloc(ptr, usize_new)  ;

        if ( ret == NULL ) {
                Pfprintf(stderr, 
                   "Realloc: Memory allocation failed in %s at line %d\n" ,
                   fname, line);
                Pfprintf(stderr, 
                   "Currentry %lu bytes are used, "
                   "reallocation from %lu to %ld bytes were requested, "
                   "and maxmem was set to %lu bytes.\n",
                    (unsigned long) (memuse-usize_new +usize_old) , 
                    (unsigned long) usize_old, 
                   (unsigned long) usize_new, (unsigned long) memmax);

                 Abort(-1);

        }

#if LEAKCHK	
    int i, found = 0;   
    for (i=MAX_MEM_LINE-1;i>=0;i--){
      if ( Mem[i].add == ptr ){ 
      	  found = 1;
          // Rfprintf(stdout,"Yes!\n");
	      Mem[length].filename = fname;
          Mem[length].add = ret;
	      Mem[length].line = line;
          break;
      }
    }
    if (!found) Rfprintf(stdout,"What is to be Recalloced? %s line %d\n",fname, line);
#endif

        return( ret ) ;
}


void __Free(size_t  memsize, void* x, char const *fname, int line)
{
/*+ Function: free with remembering allocated size  +*/

        if ( x != NULL ) {
            free(x);
            Mem_unuse(memsize);
            

#if( MEM_DEBUG >= 1 )

#if ( MEM_DEBUG == 1 ) 
            if ( am_I_root_proc() ) {

#elif ( MEM_DEBUG == 2 ) 
            if ( am_I_parent() ) {

#else
            if ( am_I_child()) {
#endif

              Pfprintf(stderr, "Free %lu bytes: in %s at line %d, ", 
                       (unsigned long) memsize,fname, line);
              Mem_print(stderr);
            }
#endif
        }
        
#if LEAKCHK            
    int i, found = 0;   
    for (i=MAX_MEM_LINE-1;i>=0;i--){
      if ( Mem[i].add == x ){ 
      	  found = 1;
          // Rfprintf(stdout,"Yes!\n");
          Mem[i].line = -1;
          break;
      }
    }
    if (!found) Rfprintf(stdout,"What is freeed? %s line %d\n",fname, line);
#endif    
        
}

int Mem_print(FILE *fp)
{

        int ret;

        if ( memcheck ) {
                ret = Pfprintf(fp, "Allocated memory: %lu, "
                      "rest: %lu\n", (unsigned long) memuse, (unsigned long) memmax-memuse);
        } else {
                ret = Pfprintf(fp, "Allocated memory: %lu\n"
                      , (unsigned long) memuse);
        }
        return(ret);
}
