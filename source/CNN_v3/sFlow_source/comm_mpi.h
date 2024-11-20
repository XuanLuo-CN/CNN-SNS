/***********************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
************************************************************/

#ifndef __COMM_MPI_H
#define __COMM_MPI_H



// #ifdef PARALLEL
#define COM_Request MPI_Request
#define COM_REQUEST_NULL MPI_REQUEST_NULL
// #else
// #define COM_Request int
// #define COM_REQUEST_NULL 0
// #endif

#include <mpi.h>
#include "stdfunc.h"
#include "hddm_types.h"
#include "mem.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>

#define GRP_PARENT 0
#define GRP_CHILD  1

#define SIG_SENDDOM -1
#define SIG_RECVDOM -2
#define SIG_SENDRES -3
#define SIG_RECVRES -4
#define SIG_BREAK   -5

#define SIG_READY   -6
#define SIG_RECV_OK -8
#define SIG_SEND    -9

#define SIG_HAVEDOM -10
#define SIG_NODOM   -11
#define SIG_CH_PAR  -12

typedef struct {
  int  group;      /*+ GroupID              +*/
  int  mygprocid;  /*+ ProcessID in a group +*/
  int  nparent;    /*+ num. of parents      +*/
  int  nchild;     /*+ num. of children     +*/
  int  npart ;     /*+ num. of parts        +*/
  int  rootid;     /*+ ProcID of root       +*/
  int *parid;      /*+ ProcIDs of parents   +*/
  int *chiid;      /*+ ProcIDs of children  +*/
} COM_GRP_ID;

typedef struct {
  int nproc;      /*+ num. of PE +*/
  int myprocid;   /*+ ProcID +*/
  int *allid ;
} COM_WLD_ID;

int  COM_Init(int * pargc, char *** pargv) ;
void COM_SetGroup ( int npart ) ;
int COM_Wait(COM_Request *request);
int  COM_Bcast_Int(int *buf, int count, int root) ;
int  COM_Bcast_Dbl(double *buf, int count, int root) ;

int  is_my_procid(int pid) ;
int  is_my_part(int ipart) ;
int  am_I_root_proc(void) ;
int  am_I_parent(void) ;
int  am_I_child(void) ;
int  get_num_proc(void) ;
int  get_num_parent(void) ;
int  get_num_child(void) ;
int  get_num_part(void) ;
int  get_rootid(void) ;
int  get_parid(int iparent);
int  get_chiid(int ichild);
int  get_myid(void) ;
int  get_group_myid(void) ;
int  get_mygroup(void) ;
int  gpart2mypart(int gpart_id);
int  get_my_npart(void);

int  COM_Synchronize(void) ;
int  COM_Grp_Synchronize(void) ;
int  COM_Abort(int errorno);
int  COM_Finalize(void) ;
void COM_Exit(int status);

int Pfprintf(FILE *fp, const char *format, ... );
int Rfprintf(FILE *fp, const char *format, ... );

int COM_ReadInt( int *buf, int size, int *node, int *type ) ;

int COM_ReadLong( int *buf, int size, int *node, int *type ) ;

int COM_ReadDbl( double *buf, int size, int *node, int *type ) ;

int COM_WriteInt( int *buf, int size, int node, int type ) ;
int COM_WriteDbl( double *buf, int size, int node, int type ) ;

int COM_GrpSumDbl( double * sendbuf, double * recvbuf, int count);
int COM_GrpMaxDbl( double * sendbuf, double * recvbuf, int count);

int COM_GrpSumInt( int * sendbuf, int * recvbuf, int count);
int COM_GrpMaxInt( int * sendbuf, int * recvbuf, int count);

int COM_MkBcondStruct(void);
int COM_ReadnBcond( Bcond * buf, int size, int * node, int * type ) ;
int COM_WritenBcond( Bcond *buf, int size, int node, int type ) ;

int COM_MkInbcStruct(void) ;
int COM_ReadnInbc( Inbc * buf, int size, int * node, int * type ) ;
int COM_WritenInbc( Inbc *buf, int size, int node, int type ) ;

int COM_MkMatStruct(void) ;
int COM_ReadnMat( Material * buf, int size, int * node, int * type ) ;
int COM_WritenMat( Material *buf, int size, int node, int type ) ;

int COM_Grp_mkdir_for_write( char * dir) ;


/**********************************************************/
/*  picked up from Solid Solver                           */
/**********************************************************/

int COM_GrpAllgather_Int(int *sendbuf, int sendcount, int *recvbuf, int recvcount);

int COM_get_group_procid_of_parent(int iparent);

int COM_Allgather_Int(int *sendbuf, int sendcount, int *recvbuf, int recvcount);

int COM_Gatherv_Dbl(double *sendbuf, int sendcount,  double *recvbuf, int *recvcounts, int stride, int root);

int COM_Scatterv_Int(int *sendbuf, int *sendcounts, int *recvbuf, int recvcount, int stride, int root);

int COM_Scatterv_Dbl(double *sendbuf, int *sendcounts, double *recvbuf, int recvcount, int stride, int root);

int COM_get_num_procs(void);

int COM_get_group_procid_of_child(int ichild);

int COM_Test(COM_Request *request);

COM_Request COM_IwritenInt(int *buf, int size, int node, int type);

COM_Request COM_IwritenLong(int *buf, int size, int node, int type);

COM_Request COM_IwritenDbl(double *buf, int size, int node, int type);

int COM_Iprobe(int *node, int *type);

int COM_GrpBcast_Int(int *buf, int count, int root);

int COM_Sendrecv_Int(int *sbuf, int *rbuf, int count, int stag, int rtag, int dest);


extern COM_WLD_ID COM_ID;

#endif /* __COMM_MPI_H */













