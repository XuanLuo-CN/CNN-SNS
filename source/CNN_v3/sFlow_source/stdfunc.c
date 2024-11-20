/***********************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
************************************************************/

#include <math.h>
#include "stdfunc.h"
#include "mem.h"
#include "hddm_types.h"

static struct timeval start_time;

/*----------------------------------------------*/

struct timeval  SetTimer(void)
{
  /*+ Function:  Initialize a timer +*/

        gettimeofday(&start_time,NULL) ;
        return(start_time);
}

/*----------------------------------------------*/

double Timer(void)
{
  /*+ Function:  Get elapsed time  +*/
        struct timeval tp ;
        double now;

        gettimeofday(&tp, NULL) ;
        now = (double) (tp.tv_sec  - start_time.tv_sec)  
             +(double) (tp.tv_usec - start_time.tv_usec)  *1.0e-6;
        return(now);

}

/*----------------------------------------------*/

void GetCPUTimes(double *u_time, double *s_time)
{
     struct tms buf;

     times(&buf);
     *u_time = (double) buf.tms_utime /_SC_CLK_TCK;
     *s_time = (double) buf.tms_stime /_SC_CLK_TCK;

}

/*----------------------------------------------*/

char *CDate(void)
{
  /*+ Function:  Output current date  +*/
  time_t t;
  
  time(&t);
  return((char*)ctime(&t));
}

/*----------------------------------------------*/

int mkdir_for_write( char * dir)
{

    struct stat statbuf;

    if ( ( stat(dir, &statbuf) != 0 ) && ( errno == ENOENT)) {
                mkdir(dir, 0777);
    }

    if ( stat(dir, &statbuf) == 0 ) {
      if (S_ISDIR(statbuf.st_mode) ) {
	if ( statbuf.st_mode & S_IWUSR ) {
                   return(0);
        } else {
                   Pfprintf(stderr, "Directory exists, but not writable: %s",
                                     dir );
                   return(1);
        }
      } else {
               Pfprintf(stderr, "Cannot make directory, file exists: %s\n", 
                        dir); 
               return(1);
      }
    }  else {
               Pfprintf(stderr, "Cannot make directory: %s\n", dir);
               return(1);
    }

}

/*----------------------------------------------*/

void d_clear( double  ar[], int n, double val )
{
  int   i ;

  for ( i = 0 ; i < n ; i++ )
    ar[i] = val ;
}

void d_arrcpy( double ar_from[], double ar_to[], int n )
{
  int  i ;

  for ( i = 0 ; i < n ; i++ )
    ar_to[i] = ar_from[i] ;
}

/**********************************************************

Function for DEBUG_HDDM
                                                           
**********************************************************/

static FILE *debug ;

void RealDebugInit (void)
{
  /*+ Function: open debug file +*/
  
  char fname[256], dir[256] ;
  int  my_group, my_group_id ;

  strcpy( dir, "debug") ;

  if (COM_Grp_mkdir_for_write(dir) > 0 ) {
    Abort(-1);
  }

  my_group = get_mygroup() ;
  my_group_id = get_group_myid() ;


  if ( my_group == GRP_PARENT ) {
    sprintf( fname, "%s/par_%d", dir, my_group_id ) ;
  } else {
    sprintf( fname, "%s/chi_%d", dir, my_group_id ) ;
  }

  if((debug = fopen ( fname, "w" )) == NULL){
    fprintf(stderr, "debug %d file do not open\n", my_group_id);
  }
}

/*----------------------------------------------*/

void RealDebugFinish (void)
{
  /*+ Function: close debug file +*/

  fclose(debug) ;

}

/*----------------------------------------------*/

void RealDprintf( const char *format, ... ) 
{
  /*+  Function:  printf with DEBUG mode +*/

        va_list ap;
        va_start( ap, format );
        vfprintf(debug, format, ap);
        va_end(ap);
        fflush(debug);

}

/***********************************************************************/


/*
       %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%          
       %% <<<<<<<<<<<<<<<<<<<                                %%          
       %%           Functions for Vector Operation           %%          
       %%                                >>>>>>>>>>>>>>>>>>> %%          
       %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%          
                                                                       */
/***********************************************************************/
int iary_op_sca(int *ary, int size, int op, int *_val) {

  int i, ret, aval;
  int val = *_val;
  
  if (size < 1 )  return(-1) ;
  
  switch (op) {
  case OP_SUB: 
    for (i=0; i<size; i++) ary[i] = val;
    ret = size ;
    break;
  case OP_SUM: 
    for (i=0; i<size; i++) ary[i] += val;
    ret = size ;
    break;
  case OP_DIF: 
    for (i=0; i<size; i++) ary[i] -= val;
    ret = size ;
    break;
  case OP_PRD: 
    for (i=0; i<size; i++) ary[i] *= val;
    ret = size ;
    break;
  case OP_DIV: 
    for (i=0; i<size; i++) ary[i] /= val;
    ret = size ;
    break;
  case OP_MAX:
    val = ary[0]; ret = 0;
    for ( i=1; i< size; i++) {
      if ( ary[i] > val ) { 
	val = ary[i];  
	ret = i; 
      }
    } 
    *_val = val;
    break;
  case OP_MIN:
    val = ary[0]; ret = 0;
    for (i=1; i< size; i++) {
      if ( ary[i] < val ) { 
	val = ary[i];  
	ret = i; 
      }
    } 
    *_val = val; 
    break;
  case OP_ABSMAX:
    val = abs(ary[0]); ret = 0;
    for (i=1; i< size; i++) {
      aval = abs(ary[i]) ;
      if ( aval > val ) { 
	val = aval;  
	ret = i; 
      }
    } 
    *_val = val;
    break;
  case OP_ABSMIN:
    val = abs(ary[0]); ret = 0;
    for (i=1; i< size; i++) {
      aval = abs(ary[i]) ;
      if ( aval < val ) { 
	val = aval;  
	ret = i; 
      }
    } 
    *_val = val;
    break;
  default:
    fprintf(stderr, "Unknown operation %d in iary_op_sca\n", op);
    exit(-1);
  }

  return(ret);
}

/*еееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееее*/

int dary_op_sca(double *ary, int size, int op, double *_val) {
  
  int i, ret;
  double aval;
  double val = *_val;
  
  if (size < 1 )  return(-1) ;
  
  switch (op) {
  case OP_SUB: 
    for (i=0; i<size; i++) ary[i] = val;
    ret = size ;
    break;
  case OP_SUM: 
    for (i=0; i<size; i++) ary[i] += val;
    ret = size ;
    break;
  case OP_DIF: 
    for (i=0; i<size; i++) ary[i] -= val;
    ret = size ;
    break;
  case OP_PRD: 
    for (i=0; i<size; i++) ary[i] *= val;
    ret = size ;
    break;
  case OP_DIV: 
    for (i=0; i<size; i++) ary[i] /= val;
    ret = size ;
    break;
  case OP_MAX:
    val = ary[0]; ret = 0;
    for (i=1; i< size; i++) {
      if ( ary[i] > val ) { 
	val = ary[i];  
	ret = i;
      }
    } 
    *_val = val;
    break;
  case OP_MIN:
    val = ary[0]; ret = 0;
    for (i=1; i< size; i++) {
      if ( ary[i] < val ) { 
	val = ary[i];  
	ret = i;
      }
    } 
    *_val = val;
    break;
  case OP_ABSMAX:
    val = fabs(ary[0]); ret = 0;
    for (i=1; i< size; i++) {
      aval = fabs(ary[i]) ;
      if ( aval > val ) { 
	val = aval;  
	ret = i;
      }
    } 
    *_val = val;
    break;
  case OP_ABSMIN:
    val = fabs(ary[0]); ret = 0;
    for (i=1; i< size; i++) {
      aval = fabs(ary[i]) ;
      if ( aval < val ) { 
	val = aval;  
	ret = i;
      }
    } 
    *_val = val;
    break;
  default:
    fprintf(stderr, "Unknown operation %d in dary_op_sca\n", op);
    exit(-1);
  }

  return(ret);
}

/*еееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееее*/

int iary_op_ary(int *ary1, int size, int op, int *ary2) {
  int i, ret;
  
  if (size < 1 )  return(-1) ;
  
  switch (op) {
  case OP_SUB: 
    for (i=0; i<size; i++) ary1[i] = ary2[i];
    ret = size ;
    break;
  case OP_SUM: 
    for (i=0; i<size; i++) ary1[i] += ary2[i];
    ret = size ;
    break;
  case OP_DIF: 
    for (i=0; i<size; i++) ary1[i] -= ary2[i];
    ret = size ;
    break;
  case OP_PRD: 
    for (i=0; i<size; i++) ary1[i] *= ary2[i];
    ret = size ;
    break;
  case OP_DIV: 
    for (i=0; i<size; i++) ary1[i] /= ary2[i];
    ret = size ;
    break;
  default:
    fprintf(stderr, "Unknown operation %d in iary_op_ary\n", op);
    exit(-1);
  }
  
  return(ret);
}

/*еееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееее*/

int dary_op_ary(double *ary1, int size, int op, double *ary2) {
  int i, ret;
       
  if (size < 1 )  return(-1) ;
  
  switch (op) {
  case OP_SUB: 
    for (i=0; i<size; i++) ary1[i] = ary2[i];
    ret = size ;
    break;
  case OP_SUM: 
    for (i=0; i<size; i++) ary1[i] += ary2[i];
    ret = size ;
    break;
  case OP_DIF: 
    for (i=0; i<size; i++) ary1[i] -= ary2[i];
    ret = size ;
    break;
  case OP_PRD: 
    for (i=0; i<size; i++) ary1[i] *= ary2[i];
    ret = size ;
    break;
  case OP_DIV: 
    for (i=0; i<size; i++) ary1[i] /= ary2[i];
    ret = size ;
    break;
  default:
    fprintf(stderr, "Unknown operation %d in dary_op_ary\n", op);
    exit(-1);
  }

  return(ret);
}

/*еееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееее*/

int iary_inner_prod(int *ary1, int size, int *ary2) {
  int i, ret;

  ret = 0;
  if (size < 1 )  return(0) ;
  
  for (i=0; i<size; i++) { 
    ret += ary1[i] * ary2[i]; 
  }
  
  return(ret);
}
double dary_inner_prod(double *ary1, int size, double *ary2) {
  int i;
  double  ret;
  
  ret = 0.0;
  if (size < 1 )  return(ret) ;
  
  for (i=0; i<size; i++) { 
    ret += ary1[i] * ary2[i];
  }
  
  return(ret);
}

/*еееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееее*/

int iary_ary_op_sca(int *ary1, int size, int *ary2, int op, int *_val) {
  int i, ret;
  int val = *_val;

  if (size < 1 )  return(-1) ;
  
  switch (op) {
  case OP_SUM: 
    for (i=0; i<size; i++) {
      ary1[i] = ary2[i] + val; 
    }
    ret = size ;
    break;
  case OP_DIF: 
    for (i=0; i<size; i++) {
      ary1[i] = ary2[i] - val; 
    }
    ret = size ;
    break;
  case OP_PRD: 
    for (i=0; i<size; i++) {
      ary1[i] = ary2[i] * val; 
    }
    ret = size ;
    break;
  case OP_DIV: 
    for (i=0; i<size; i++) {
      ary1[i] = ary2[i] / val; 
    }
    ret = size ;
    break;
  default:
    fprintf(stderr, "Unknown operation %d in iary_ary_op_sca\n", op);
    exit(-1);
  }
  
  return(ret);
}

/*еееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееее*/

int dary_ary_op_sca( double * ary1, int size, double *ary2, 
	                                            int op , double *_val) {
  int i, ret;
  double val = *_val;

  if (size < 1 )  return(-1) ;
  
  switch (op) {
  case OP_SUM: 
    for (i=0; i<size; i++) {
      ary1[i] = ary2[i] + val; 
    }
    ret = size ;
    break;
  case OP_DIF: 
    for (i=0; i<size; i++) {
      ary1[i] = ary2[i] - val; 
    }
    ret = size ;
    break;
  case OP_PRD: 
    for (i=0; i<size; i++) {
      ary1[i] = ary2[i] * val; 
    }
    ret = size ;
    break;
  case OP_DIV: 
    for (i=0; i<size; i++) {
      ary1[i] = ary2[i] / val; 
    }
    ret = size ;
    break;
  default:
    fprintf(stderr, "Unknown operation %d in dary_ary_op_sca\n", op);
    exit(-1);
  }

  return(ret);
}

/*еееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееее*/
int iary_ary_op_ary(int *ary1, int size, int *ary2, int op , int *ary3) {
  int i, ret;

  if (size < 1 )  return(-1) ;
  
  switch (op) {
  case OP_SUM: 
    for (i=0; i<size; i++) {
      ary1[i] = ary2[i] + ary3[i]; 
    }
    ret = size ;
    break;
  case OP_DIF: 
    for (i=0; i<size; i++) {
      ary1[i] = ary2[i] - ary3[i]; 
    }
    ret = size ;
    break;
  case OP_PRD: 
    for (i=0; i<size; i++) {
      ary1[i] = ary2[i] * ary3[i]; 
    }
    ret = size ;
    break;
  case OP_DIV: 
    for (i=0; i<size; i++) {
      ary1[i] = ary2[i] / ary3[i]; 
    }
    ret = size ;
    break;
  default:
    fprintf(stderr, "Unknown operation %d in iary_ary_op_ary\n", op);
    exit(-1);
  }
  
  return(ret);
}

/*еееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееее*/

int dary_ary_op_ary(double *ary1, int size, double *ary2, 
	                                           int op , double *ary3)  {
  int i, ret;

  if (size < 1 )  return(-1) ;

  switch (op) {
  case OP_SUM: 
    for (i=0; i<size; i++) {
      ary1[i] = ary2[i] + ary3[i]; 
    }
    ret = size ;
    break;
  case OP_DIF: 
    for (i=0; i<size; i++) {
      ary1[i] = ary2[i] - ary3[i]; 
    }
    ret = size ;
    break;
  case OP_PRD: 
    for (i=0; i<size; i++) {
      ary1[i] = ary2[i] * ary3[i]; 
    }
    ret = size ;
    break;
  case OP_DIV: 
    for (i=0; i<size; i++) {
      ary1[i] = ary2[i] / ary3[i]; 
    }
    ret = size ;
    break;
  default:
    fprintf(stderr, "Unknown operation %d in dary_ary_op_ary\n", op);
    exit(-1);
  }

  return(ret);
}


/***********************************************************************/



void SkylineMat_Asym_to_Sym( AsymSkylineMatrix mat_Asym , SkylineMatrix mat_Sym )
{
 return;

}

void SkylineMat_Sym_to_Asym( SkylineMatrix mat_Sym , AsymSkylineMatrix mat_Asym )
{
 return;

}






