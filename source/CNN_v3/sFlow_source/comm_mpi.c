/***********************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
************************************************************/


#include "comm_mpi.h"
#include "mem.h"
#include "hddm_types.h"
#include "defs.h"
#include "stdfunc.h"


COM_WLD_ID COM_ID;
COM_GRP_ID COM_GID;

static MPI_Datatype COM_Dtype_Bcond;
static MPI_Datatype COM_Dtype_Inbc;
static MPI_Datatype COM_Dtype_Mat;
static MPI_Comm  COM_GroupID_me;

#define BUFSIZE 64*1024*1024


int COM_mkdir_for_write( char * dir, MPI_Comm comm) ;

int  COM_Init(int * pargc, char *** pargv)
{
  /*+  Function:  Initialize communication  +*/

  int ret, nproc, i, myprocid;
  
  ret = MPI_Init(pargc,  pargv) ;
  MPI_Comm_rank(MPI_COMM_WORLD, &myprocid);
  MPI_Comm_size(MPI_COMM_WORLD, &nproc);
  COM_MkBcondStruct();
  COM_MkInbcStruct() ;
  COM_MkMatStruct() ;

  COM_ID.allid = (int *) calloc(sizeof(int), nproc);  
  /*   MPI's rank is 0 to nproc */
  for (i = 0; i< nproc; i++) {
              COM_ID.allid[i] = i;
  }

  COM_ID.nproc = nproc;
  COM_ID.myprocid = myprocid;

  return(ret);

}

/**********************************************************/
void COM_SetGroup ( int npart ) 
{

  int procid, nproc, nparent, nchild ;

  procid = COM_ID.myprocid ;
  nproc = COM_ID.nproc ;
  nparent = npart ;

  nchild = nproc - nparent ;

#ifdef PARA_PART
  if ( nchild != 0 ) {
    fprintf(stderr, "Error: In PARALLEL-mode, nparts (%d) must "
            "be equal to nproc (%d)\n",  nparent, nproc);
    COM_Abort(-1);
  }
#endif
#ifdef PARA_HDDM
  if ( nchild <= 0 ) {
    fprintf(stderr, "Error: too few %d process(es) " 
            "for %d parent(s)\n", nproc, nparent);
    COM_Abort(-1);
  }
#endif

  COM_GID.nparent = npart   ;
  COM_GID.npart   = npart   ;
  COM_GID.nchild  = nchild  ;

  COM_GID.parid  =  COM_ID.allid ;
  COM_GID.chiid  =  COM_ID.allid + nparent ;

  COM_GID.rootid = COM_GID.parid[0] ;

  if ( COM_ID.myprocid < nparent ) {
    COM_GID.group = GRP_PARENT;
    COM_GID.mygprocid = procid ;
    MPI_Comm_split(MPI_COMM_WORLD, 0, procid, &COM_GroupID_me);
    
  } else {
    COM_GID.group = GRP_CHILD;
    COM_GID.mygprocid = procid - nparent;
    MPI_Comm_split(MPI_COMM_WORLD, 1, procid, &COM_GroupID_me);
  }

}

/**********************************************************/
int COM_Bcast_Int(int *buf, int count, int root)
{
  /*+  Function:  Broadcast integer to all +*/

        return(MPI_Bcast(buf, count, MPI_INT, root, MPI_COMM_WORLD )) ;

}

/**********************************************************/
int COM_Bcast_Dbl(double *buf, int count, int root)
{
  /*+  Function:  Broadcast integer to all +*/

        return(MPI_Bcast(buf, count, MPI_DOUBLE, root, MPI_COMM_WORLD )) ;

}

/**********************************************************/
int is_my_procid(int pid)
{
  /*+  Function:  Is pid my pid? +*/

  if (COM_ID.myprocid == pid ) {
            return(1);
  } else {
            return(0);
  }
}

/**********************************************************/
int is_my_part(int ipart)
{
  /*+  Function:  Is ipart my part ID? +*/
 
  if ( (COM_GID.group==GRP_PARENT) && (COM_GID.mygprocid==ipart) ){ 
    return(1);
  } else {
    return(0);
  }
}

/**********************************************************/
int am_I_root_proc(void)
{
  /*+  Function:  Am I root process? +*/

  if (COM_ID.myprocid == 0 ) {
            return(1);
  } else {
            return(0);
  }
}

/**********************************************************/

int am_I_parent(void)
{
  /*+  Function:  Am I in parent group? +*/

  if (COM_GID.group == GRP_PARENT ) {
            return(1);
  } else {
            return(0);
  }
}

/**********************************************************/
int am_I_child(void)
{
  /*+  Function:  Am I in child group? +*/

  if (COM_GID.group == GRP_CHILD ) {
            return(1);
  } else {
            return(0);
  }
}

/**********************************************************/
int get_num_proc(void)
{
  /*+  Function:  Return number of proc. +*/
  return (COM_ID.nproc) ;
}

/**********************************************************/
int get_num_parent(void)
{
  /*+  Function:  Return number of parents +*/
  return (COM_GID.nparent) ;
}

/**********************************************************/
int get_num_child(void)
{
  /*+  Function:  Return number of childs +*/
  return (COM_GID.nchild) ;
}

/**********************************************************/
int get_num_part(void)
{
  /*+  Function:  Return number of parts +*/
  return (COM_GID.npart) ;
}

/**********************************************************/
int get_rootid(void)
{
  /*+  Function:  Return ID of root +*/
  return (COM_GID.rootid) ;
}

/**********************************************************/
int get_parid(int iparent)
{
  /*+  Function:  Return ID of parent[iparent] +*/
  return (COM_GID.parid[iparent]) ;
}

/**********************************************************/
int get_chiid(int ichild)
{
  /*+  Function:  Return ID of child[ichild] +*/
  return (COM_GID.chiid[ichild]) ;
}

/**********************************************************/
int  get_myid(void)
{
  /*+  Function: Retrun My ID +*/
  return ( COM_ID.myprocid ) ;

}

/**********************************************************/
int  get_group_myid() 
{
  /*+  Function:  Return My GroupID +*/
  return (COM_GID.mygprocid) ;
}

/**********************************************************/
int  get_mygroup() 
{
  return (COM_GID.group) ;
}

/**********************************************************/
int gpart2mypart(int gpart_id)
{
  /*+  Function:  Get My Part ID from Global Part ID +*/

       if ( is_my_part(gpart_id) ) {
              return (0);
       } else {
              return (-1) ;
       }
       
}

/**********************************************************/
int get_my_npart(void)
{
  /*+  Function:  How many parts does this process have  +*/

  if (am_I_parent()) {
    return (1);
  } else {
    return (0) ;
  }
  
}

/**********************************************************/
int  COM_Synchronize(void)
{
  /*+  Function:  Synchronize all processes +*/

        return( MPI_Barrier(MPI_COMM_WORLD));

}

/**********************************************************/
int  COM_Grp_Synchronize(void)
{
  /*+  Function:  Synchronize processes in own group +*/

        return( MPI_Barrier(COM_GroupID_me));

}


/**********************************************************/
int  COM_Abort(int errorno)
{
  /*+  Function:  Abort all processes +*/

        return( MPI_Abort(MPI_COMM_WORLD,errorno));

}
/**********************************************************/
int  COM_Finalize(void)
{
  MPI_Type_free(&COM_Dtype_Bcond);
  MPI_Type_free(&COM_Dtype_Inbc);
  MPI_Type_free(&COM_Dtype_Mat);
  free(COM_ID.allid);
  return(MPI_Finalize());

}

/**********************************************************/
void  COM_Exit(int status)
{
  /*+  Function:  Exit +*/

        MPI_Finalize();
        exit(status);
}

/**********************************************************/
int Pfprintf(FILE *fp, const char *format, ... ) 
{
  /*+  Function:  fprintf with [ProcID] +*/

        static char buf[1024];
        va_list ap;
        int ret;

        sprintf(buf, "[%d] ", COM_ID.myprocid );
        strncat(buf, format, 1023);

        va_start( ap, format );

        ret = vfprintf(fp, buf, ap);
        va_end(ap);

        fflush(fp);

        return(ret);
}

/**********************************************************/
int Rfprintf(FILE *fp, const char *format, ... ) 
{
  /*+  Function:  fprintf only if I am root parent +*/

        va_list ap;

        if (am_I_root_proc()){
            va_start( ap, format );
            vfprintf(fp, format, ap);
            va_end(ap);
            fflush(fp);
        }

        return(0);

}


/**********************************************************/

int COM_ReadInt( int *buf, int size, int *node, int *type )
{
  MPI_Status status ;
  int rv ;
  
  if ( *node == -1 ) *node = MPI_ANY_SOURCE ;
  if ( *type == -1 ) *type = MPI_ANY_TAG ;

  rv = MPI_Recv( buf, size, MPI_INT, *node, *type,
                MPI_COMM_WORLD, &status ) ;

  if ( rv ) {
    fprintf( stderr, "(%x) COM_ReadInt - error#%d from %x %d\n",
            COM_ID.myprocid, rv, *node, *type) ;
    pause() ;
  }

  *node = status.MPI_SOURCE ;
  *type = status.MPI_TAG ;

  return(rv);

}

/**********************************************************/

int COM_ReadLong( int *buf, int size, int *node, int *type )
{
  MPI_Status status ;
  int rv ;
  
  if ( *node == -1 ) *node = MPI_ANY_SOURCE ;
  if ( *type == -1 ) *type = MPI_ANY_TAG ;

  rv = MPI_Recv( buf, size, MPI_LONG, *node, *type,
                MPI_COMM_WORLD, &status ) ;

  if ( rv ) {
    fprintf( stderr, "(%x) COM_ReadLong - error#%d from %x %d\n",
            COM_ID.myprocid, rv, *node, *type) ;
    pause() ;
  }

  *node = status.MPI_SOURCE ;
  *type = status.MPI_TAG ;

  return(rv);

}

/**********************************************************/

int COM_ReadDbl( double *buf, int size, int *node, int *type )
{
  MPI_Status status ;
  int rv ;

  if ( *node == -1 ) *node = MPI_ANY_SOURCE ;
  if ( *type == -1 ) *type = MPI_ANY_TAG ;

  rv = MPI_Recv( buf, size, MPI_DOUBLE, *node, *type,
                MPI_COMM_WORLD, &status ) ;

  if ( rv ) {
    fprintf( stderr, "(%x) COM_ReadDbl - error#%d from %x %d\n",
            COM_ID.myprocid, rv, *node, *type) ;
    pause() ;
  }

  *node = status.MPI_SOURCE ;
  *type = status.MPI_TAG ;

  return(rv);

}

/**********************************************************/

int COM_WriteInt( int *buf, int size, int node, int type )
{
  int rv ;

  rv = MPI_Send( buf, size, MPI_INT, node, type, MPI_COMM_WORLD ) ;

  if ( rv ) {
    fprintf( stderr, "(%x) COM_WriteInt - error#%d to %x %d\n",
            COM_ID.myprocid, rv, node, type) ;
    pause() ;
  }

  return(rv);

}

/**********************************************************/

int COM_WriteDbl( double *buf, int size, int node, int type )
{
  int rv ;
  rv = MPI_Send( buf, size, MPI_DOUBLE, node, type, MPI_COMM_WORLD ) ;

  if ( rv < 0 ) {
    fprintf( stderr, "(%x) COM_WriteDbl - error#%d to %x %d\n",
            COM_ID.myprocid, rv, node, type) ;
    pause() ;
  }

  return(rv);

}


/**********************************************************/
int COM_GrpSumDbl( double * sendbuf, double * recvbuf, int count)
{
  /*+  Function:  Sum up double in a group +*/

#if 1
        return(MPI_Allreduce(sendbuf, recvbuf, count, MPI_DOUBLE , MPI_SUM, 
                     COM_GroupID_me));
#else
        return(MPI_Allreduce(sendbuf, recvbuf, count, MPI_DOUBLE , MPI_SUM, 
                     MPI_COMM_WORLD));
#endif
}

/**********************************************************/
int COM_GrpMaxDbl( double * sendbuf, double * recvbuf, int count)
{
  /*+  Function:  Get maximum double in a group +*/
#if 1
        return(MPI_Allreduce(sendbuf, recvbuf, count, MPI_DOUBLE , MPI_MAX, 
                     COM_GroupID_me));
#else
        return(MPI_Allreduce(sendbuf, recvbuf, count, MPI_DOUBLE , MPI_MAX, 
                     MPI_COMM_WORLD));
#endif

}

/**********************************************************/
int COM_GrpSumInt( int * sendbuf, int * recvbuf, int count)
{
  /*+  Function:  Sum up integer in a group +*/
#if 1
        return(MPI_Allreduce(sendbuf, recvbuf, count, MPI_INT, MPI_SUM, 
                     COM_GroupID_me));
#else
        return(MPI_Allreduce(sendbuf, recvbuf, count, MPI_INT, MPI_SUM, 
                     MPI_COMM_WORLD));
#endif
}

/**********************************************************/
int COM_GrpMaxInt( int * sendbuf, int * recvbuf, int count)
{
  /*+  Function:  Get maximum integer in a group +*/

#if 1
          return(MPI_Allreduce(sendbuf, recvbuf, count, MPI_INT , MPI_MAX, 
                     COM_GroupID_me));
#else
        return(MPI_Allreduce(sendbuf, recvbuf, count, MPI_INT , MPI_MAX, 
                     MPI_COMM_WORLD));
#endif

}


/**********************************************************/
int COM_MkBcondStruct(void){
  /*+  Function:  Define struct for communication of Bcond +*/

  int count=3;
  int blengths[3];
  MPI_Aint displacements[3];
  MPI_Datatype datatypes[3];
  Bcond bc;

  blengths[0] = 1; 
  displacements[0] = (MPI_Aint)(&bc.node) - (MPI_Aint)(&bc);
  datatypes[0] = MPI_INT;

  blengths[1] = 1; 
  displacements[1] = (MPI_Aint)(&bc.co) - (MPI_Aint)(&bc);
  datatypes[1] = MPI_INT;

  blengths[2] = 1; /* one data */
  displacements[2] = (MPI_Aint)(&bc.val) - (MPI_Aint)(&bc);
  datatypes[2] = MPI_DOUBLE;

  MPI_Type_struct(count, blengths, displacements, 
                  datatypes, &COM_Dtype_Bcond) ;

  return( MPI_Type_commit(&COM_Dtype_Bcond) );


}

/**********************************************************/
int COM_ReadnBcond( Bcond * buf, int size, int * node, int * type )
{
  /*+  Function:  Receive type Bcond +*/

  MPI_Status status ;
  int rv;

  if ( *node == -1 ) *node = MPI_ANY_SOURCE ;
  if ( *type == -1 ) *type = MPI_ANY_TAG ;

  rv = MPI_Recv( buf, size, COM_Dtype_Bcond, *node, *type,
                 MPI_COMM_WORLD, &status ) ;

  if ( rv ) {
    fprintf( stderr, "(%d) readn_bcnd - error#%d from %d %d\n",
             COM_ID.myprocid, rv, *node, *type) ;
    COM_Abort(-1);
  }

  *node = status.MPI_SOURCE ;
  *type = status.MPI_TAG ;

  return(rv);
}

/**********************************************************/
int COM_WritenBcond( Bcond *buf, int size, int node, int type )
{
  /*+  Function:  Send type Bcond +*/

  int rv ;

  rv = MPI_Send( buf, size, COM_Dtype_Bcond, node, type, MPI_COMM_WORLD ) ;

  if ( rv < 0 ) {
    fprintf( stderr, "(%d) writen_bcnd - error#%d to %d %d\n",
             COM_ID.myprocid, rv, node, type) ;
    COM_Abort(-1);
  }

  return(rv);
}

/**********************************************************/
int COM_MkInbcStruct(void)
{
  /*+  Function:  Define struct for communication of Inbc +*/

  int count=4;
  int blengths[4];
  MPI_Aint displacements[4];
  MPI_Datatype datatypes[4];
  Inbc inbc;

  blengths[0] = 1; 
  displacements[0] = (MPI_Aint)(&inbc.aspart) - (MPI_Aint)(&inbc);
  datatypes[0] = MPI_INT;

  blengths[1] = 1; 
  displacements[1] = (MPI_Aint)(&inbc.mypart_ibid) - (MPI_Aint)(&inbc);
  datatypes[1] = MPI_INT;

  blengths[2] = 1; 
  displacements[2] = (MPI_Aint)(&inbc.node) - (MPI_Aint)(&inbc);
  datatypes[2] = MPI_INT;

  blengths[3] = 1; 
  displacements[3] = (MPI_Aint)(&inbc.co) - (MPI_Aint)(&inbc);
  datatypes[3] = MPI_INT;

  MPI_Type_struct(count, blengths, displacements, 
                  datatypes, &COM_Dtype_Inbc);
  return( MPI_Type_commit(&COM_Dtype_Inbc) );


}

/**********************************************************/
int COM_ReadnInbc( Inbc * buf, int size, int * node, int * type )
{
  /*+  Function:  Receive type Inbc +*/

  MPI_Status status ;
  int rv;

  if ( *node == -1 ) *node = MPI_ANY_SOURCE ;
  if ( *type == -1 ) *type = MPI_ANY_TAG ;

  rv = MPI_Recv( buf, size, COM_Dtype_Inbc, *node, *type,
                 MPI_COMM_WORLD, &status ) ;

  if ( rv ) {
    fprintf( stderr, "(%d) readn_bcnd - error#%d from %d %d\n",
             COM_ID.myprocid, rv, *node, *type) ;
    COM_Abort(-1);
  }

  *node = status.MPI_SOURCE ;
  *type = status.MPI_TAG ;

  return(rv);
}

/**********************************************************/
int COM_WritenInbc( Inbc *buf, int size, int node, int type )
{
  /*+  Function:  Send type Inbc +*/

  int rv ;

  rv = MPI_Send( buf, size, COM_Dtype_Inbc, node, type, MPI_COMM_WORLD ) ;

  if ( rv < 0 ) {
    fprintf( stderr, "(%d) writen_inbc - error#%d to %d %d\n",
             COM_ID.myprocid, rv, node, type) ;
    COM_Abort(-1);
  }

  return(rv);
}


/**********************************************************/
int COM_MkMatStruct(void){
  /*+  Function:  Define struct for communication of Mat +*/

  int count=2;
  int blengths[2] ;
  MPI_Aint displacements[2] ;
  MPI_Datatype datatypes[2] ;
  Material mat;

  blengths[0] = 1; 
  displacements[0] =( MPI_Aint )(&mat.nu) - (MPI_Aint)(&mat) ;
  datatypes[0] = MPI_DOUBLE;

  blengths[1] = 1; 
  displacements[1] =( MPI_Aint )(&mat.del_time) - (MPI_Aint)(&mat) ;
  datatypes[1] = MPI_DOUBLE;


  MPI_Type_struct(count, blengths, displacements, 
                  datatypes, &COM_Dtype_Mat) ;

  return( MPI_Type_commit(&COM_Dtype_Mat) );


}

/**********************************************************/
int COM_ReadnMat( Material * buf, int size, int * node, int * type )
{
  /*+  Function:  Receive type Mat +*/

  MPI_Status status ;
  int rv;

  if ( *node == -1 ) *node = MPI_ANY_SOURCE ;
  if ( *type == -1 ) *type = MPI_ANY_TAG ;

  rv = MPI_Recv( buf, size, COM_Dtype_Mat, *node, *type,
                 MPI_COMM_WORLD, &status ) ;

  if ( rv ) {
    fprintf( stderr, "(%d) readn_mat - error#%d from %d %d\n",
             COM_ID.myprocid, rv, *node, *type) ;
    COM_Abort(-1);
  }

  *node = status.MPI_SOURCE ;
  *type = status.MPI_TAG ;

  return(rv);
}

/**********************************************************/
int COM_WritenMat( Material *buf, int size, int node, int type )
{
  /*+  Function:  Send type Mat +*/

  int rv ;

  rv = MPI_Send( buf, size, COM_Dtype_Mat, node, type, MPI_COMM_WORLD ) ;

  if ( rv < 0 ) {
    fprintf( stderr, "(%d) writen_mat - error#%d to %d %d\n",
             COM_ID.myprocid, rv, node, type) ;
    COM_Abort(-1);
  }

  return(rv);
}


/***********************************************************/
int COM_mkdir_for_write( char * dir, MPI_Comm comm)
{
  /*+  Function:  Make dir on procs in comm +*/

    int nproc;
    int iproc;
    int myrank;
    int not_ok = 0;
    int t_not_ok = 0;

    MPI_Comm_size(comm, &nproc);
    MPI_Comm_rank(comm, &myrank);
   
    for (iproc = 0; iproc < nproc; iproc++) {
      if ( iproc != myrank ) continue;

      not_ok = mkdir_for_write(dir);

    } /* iproc */

    MPI_Allreduce(&not_ok, &t_not_ok, 1, MPI_INT, MPI_SUM, comm );

    return(t_not_ok);

}
/**********************************************************/
int COM_Wait(MPI_Request *request) {
  MPI_Status status;
  int rv;

  rv = MPI_Wait(request, &status);

  if (rv) {
    fprintf(stderr, "(%d) wait - error#%d\n", COM_ID.myprocid, rv);
    COM_Abort(-1);
  }
  return(rv);
}

/**********************************************************/
int COM_Grp_mkdir_for_write( char * dir)
{
  /*+  Function:  Make dir on procs in a gropu +*/

   return (COM_mkdir_for_write( dir, COM_GroupID_me) ) ;
   
}

/**********************************************************/
/*  picked up from Solid Solver                           */
/**********************************************************/

/* Function: Allgather integer to all in a group */
int COM_GrpAllgather_Int(int *sendbuf, int sendcount,
			 int *recvbuf, int recvcount) {
  int rv;
  rv = MPI_Allgather(sendbuf, sendcount, MPI_INT, 
		     recvbuf, recvcount, MPI_INT, COM_GroupID_me);
  return(rv);
}

/* Function: Return my processor ID in parent group */
int COM_get_group_procid_of_parent(int iparent) {
  return(COM_GID.parid[iparent]);
}

/* Function: Allgather integer to all */
int COM_Allgather_Int(int *sendbuf, int sendcount, 
		      int *recvbuf, int recvcount) {
  int rv;
  rv = MPI_Allgather(sendbuf, sendcount, MPI_INT, 
		     recvbuf, recvcount, MPI_INT, MPI_COMM_WORLD);
  return(rv);
}

/* Function: Gather double to all */
int COM_Gatherv_Dbl(double *sendbuf, int sendcount,
		    double *recvbuf, int *recvcounts, int stride, int root)
{
  int nproc, ret;
  int* displs=NULL;
  
  if (am_I_root_proc()) {
    int i;
    MPI_Comm_size(MPI_COMM_WORLD,&nproc);
    displs = (int *)Calloc(sizeof(int), nproc);
    displs[0] = 0;
    for (i=1; i<nproc; i++)
      displs[i] = recvcounts[i-1] * stride + displs[i-1];
  }

  ret = MPI_Gatherv(sendbuf, sendcount, MPI_DOUBLE,
		    recvbuf, recvcounts, displs, MPI_DOUBLE,
		    root, MPI_COMM_WORLD);

  if (am_I_root_proc())
    Free(sizeof(int)*nproc, displs);
  return(ret);
}

/* Function: Scatter integer to all */
int COM_Scatterv_Int(int *sendbuf, int *sendcounts,
		     int *recvbuf, int recvcount, int stride, int root) {
  int nproc, ret;
  int* displs=NULL;

  if (am_I_root_proc()) {
    int i, pt=0;
    MPI_Comm_size(MPI_COMM_WORLD,&nproc);
    displs = (int *) Calloc(sizeof(int), nproc);
    for (i=0; i<nproc; i++) {
      displs[i] = pt;
      pt += sendcounts[i] * stride;
    }
  }

  ret = MPI_Scatterv(sendbuf, sendcounts, displs, MPI_INT, 
		     recvbuf, recvcount, MPI_INT,
		     root, MPI_COMM_WORLD);

  if (am_I_root_proc())
    Free(sizeof(int)*nproc, displs);
  return(ret);
}

/* Function: Scatter double to all */
int COM_Scatterv_Dbl(double *sendbuf, int *sendcounts,
		     double *recvbuf, int recvcount, int stride, int root) {
  int nproc, ret;
  int* displs=NULL;

  if (am_I_root_proc()) {
    int i, pt=0;
    MPI_Comm_size(MPI_COMM_WORLD,&nproc);
    displs = (int *) Calloc(sizeof(int), nproc);
    for (i=0; i<nproc; i++) {
      displs[i] = pt;
      pt += sendcounts[i] * stride;
    }
  }

  ret = MPI_Scatterv(sendbuf, sendcounts, displs, MPI_DOUBLE, 
		     recvbuf, recvcount, MPI_DOUBLE,
		     root, MPI_COMM_WORLD);

  if (am_I_root_proc())
    Free(sizeof(int)*nproc, displs);
  return(ret);
}

/* Function: Return number of processors */
int COM_get_num_procs(void) {
  int nproc;
  MPI_Comm_size(MPI_COMM_WORLD, &nproc);
  return(nproc);
}

/* Function: Return my processor ID in child group */
int COM_get_group_procid_of_child(int ichild) {
  return(COM_GID.chiid[ichild]);
}


/* Function: Test non-blocking communication */
int COM_Test(MPI_Request *request) {
  MPI_Status status;
  int rv, flag;

  rv = MPI_Test(request, &flag, &status);

  if (rv) {
    fprintf(stderr, "(%d) test - error#%d\n", COM_ID.myprocid, rv);
    COM_Abort(-1);
  }
  return(flag);
}


/* Function: Non-blocking send integer */
MPI_Request COM_IwritenInt(int *buf, int size, int node, int type) {
  MPI_Request request;
  int rv;

  rv = MPI_Isend(buf, size, MPI_INT, node, type, MPI_COMM_WORLD, &request);

  if (rv) {
    fprintf(stderr, "(%d) iwriten_int - error#%d to %d %d\n",
	    COM_ID.myprocid, rv, node, type);
    COM_Abort(-1);
  }
  return(request);
}

MPI_Request COM_IwritenLong(int *buf, int size, int node, int type) {
  MPI_Request request;
  int rv;

  rv = MPI_Isend(buf, size, MPI_LONG, node, type, MPI_COMM_WORLD, &request);

  if (rv) {
    fprintf(stderr, "(%d) iwriten_long - error#%d to %d %d\n",
	    COM_ID.myprocid, rv, node, type);
    COM_Abort(-1);
  }
  return(request);
}

/* Function: Non-blocking send double */
MPI_Request COM_IwritenDbl(double *buf, int size, int node, int type) {
  MPI_Request request;
  int rv;

  rv = MPI_Isend(buf, size, MPI_DOUBLE, node, type, MPI_COMM_WORLD, &request);

  if ( rv < 0 ) {
    fprintf(stderr, "(%d) iwriten_dbl - error#%d to %d %d\n",
	    COM_ID.myprocid, rv, node, type);
    COM_Abort(-1);
  }
  return(request);
}


/* Function: Probe non-blocking communication */
int COM_Iprobe(int *node, int *type) {
  MPI_Status status;
  int rv, flag;

  if ( *node == -1 ) *node = MPI_ANY_SOURCE;
  if ( *type == -1 ) *type = MPI_ANY_TAG;

  rv = MPI_Iprobe(*node, *type, MPI_COMM_WORLD, &flag, &status);

  if (rv) {
    fprintf(stderr, "(%d) iprobe - error#%d from %d %d\n",
	    COM_ID.myprocid, rv, *node, *type);
    COM_Abort(-1);
  }
  if ( flag != 0 ) {
    *node = status.MPI_SOURCE;
    *type = status.MPI_TAG;
  }
  return(flag);
}

/*+ Function: Broadcast integer to all in own group +*/
int COM_GrpBcast_Int(int *buf, int count, int root) {
  int rv;
  rv = MPI_Bcast(buf, count, MPI_INT, root, COM_GroupID_me);
  return(rv);
}

/*+ Function: Broadcast integer to all in own group +*/
int COM_Sendrecv_Int(int *sbuf, int *rbuf,int count, int stag, int rtag, int dest) {
  MPI_Status status;
  int rv;
  rv = MPI_Sendrecv(sbuf, count, MPI_INT, dest, stag, rbuf, count, MPI_INT, dest, rtag, MPI_COMM_WORLD, &status);
  
  if ( rv ) {
    fprintf( stderr, "(%x) COM_Sendrecv_Int - error#%d from %x %d\n",
            COM_ID.myprocid, rv, dest, rtag) ;
    pause() ;
  }

  dest = status.MPI_SOURCE ;
  //&rtag = status.MPI_TAG ;
  
  return(rv);
}

