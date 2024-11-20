/***********************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
************************************************************/

#ifndef __STDFUNC_H
#define __STDFUNC_H

#include "comm_mpi.h"
#include "hddm_types.h"
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>


#ifndef SINGLE

#define Abort( x ) COM_Abort( x )
#define Exit( x ) COM_Exit( x )

#else

#define Abort( x ) exit( x ) 
#define Exit( x ) exit( x )

#endif

//#define HDDM_DEBUG

struct timeval  SetTimer(void) ;
double Timer(void) ;
void   GetCPUTimes(double *u_time, double *s_time) ;
char  *CDate(void);
int    mkdir_for_write( char * dir) ;
void   d_clear( double  ar[], int n, double val ) ;
void   d_arrcpy( double ar_from[], double ar_to[], int n ) ;

/********** For Debug *****************/

#ifndef HDDM_DEBUG

void DummyDebugInit (void) ;
void DummyDebugFinish (void) ;
void DummyDprintf( const char *format, ... ) ;
#define DebugInit    1 ? (void)0 : DummyDebugInit
#define DebugFinish  1 ? (void)0 : DummyDebugFinish
#define Dprintf      1 ? (void)0 : DummyDprintf

#else

void RealDebugInit (void) ;
void RealDebugFinish (void) ;
void RealDprintf( const char *format, ... ) ;
#define DebugInit    RealDebugInit
#define DebugFinish  RealDebugFinish
#define Dprintf      RealDprintf

#endif



/*********** For Vector Operation *********************/

#define OP_SUB     1
#define OP_SUM     2
#define OP_DIF     3
#define OP_PRD     4
#define OP_DIV     5

#define OP_MAX     11
#define OP_MIN     12
#define OP_ABSMAX  13
#define OP_ABSMIN  14

int        iary_op_sca( int    *ary,  int size,               int op, int    *val ); 
int        dary_op_sca( double *ary,  int size,               int op, double *val );
int        iary_op_ary( int    *ary1, int size,               int op, int    *ary2);
int        dary_op_ary( double *ary1, int size,               int op, double *ary2); 
int    iary_inner_prod( int    *ary1, int size, int    *ary2                      );
double dary_inner_prod( double *ary1, int size, double *ary2                      );
int    iary_ary_op_sca( int    *ary1, int size, int    *ary2, int op, int    *val );
int    dary_ary_op_sca( double *ary1, int size, double *ary2, int op, double *val );
int    iary_ary_op_ary( int    *ary1, int size, int    *ary2, int op, int    *ary3);
int    dary_ary_op_ary( double *ary1, int size, double *ary2, int op, double *ary3);

/*********** For Vector Operation *********************/


void SkylineMat_Asym_to_Sym( AsymSkylineMatrix mat_Asym , SkylineMatrix mat_Sym );
void SkylineMat_Sym_to_Asym( SkylineMatrix mat_Sym , AsymSkylineMatrix mat_Asym );






#endif /* __STDFUNC_H */















