#include "get_global_mesh.h"

void get_global_mesh (GlobalMesh *global, char CASE[], char MODEL_ID[]){
Rfprintf(stderr, "get_global_mesh...\n");   
 AdvDatabox *dbox;
 AdvDocument *doc;
 adv_off_t off ;
 int  i, j, k, size, **temp, n_domain, t_elem, gel;
  int myid = get_myid();

 const char *element_type;
 char  fname[256];

 
  /************** Init ****************/
  global->nel = 0;
  global->nnodes = 0;
  global->elel = NULL;
  global->crd = NULL;  
  global->nop = NULL;  
  //   sprintf(fname, "%s/frame/global.adv", insw.data_dir);
  sprintf(fname, "/public/home/graduate/sFlow-LUO2/Neural_Network/data/unstructed_%s/%s_%s/adv/",CASE, CASE, MODEL_ID);
  dbox = adv_dbox_new() ;
  if( ! adv_dbox_add(dbox, fname) ) {
    fprintf(stderr, "Cannot open %s\n", fname);
    adv_dbox_close(dbox);
    Abort(-1);
    }
    Rfprintf(stderr, "  File: %s is opened successfully!\n", fname);
  
  /*********** Node ********************/
  if (  (doc = adv_dbox_find_by_property(dbox, NULL, "content_type", "Node", NULL) ) == NULL ) {
    Pfprintf(stderr, "Cannot find document Node in  %s\n", fname);
    Abort(-1);
  }
  adv_dio_get_property_int32(doc,"num_items", &(global->nnodes));
  size = global->nnodes * 3;
  global->crd =(double*)Calloc( sizeof(double), size );
  adv_dio_read_float64v(doc, 0, size, global->crd);
  adv_dio_close(doc);

  /************** Element ***************/
  if ( (doc = adv_dbox_find_by_property(dbox, NULL, "content_type", "Element", NULL) ) == NULL ) {
    Pfprintf(stderr, "Cannot find document Element in  %s\n", fname);
    Abort(-1);
  }
  adv_dio_get_property_int32( doc, "num_items", &(global->nel) );
  element_type = adv_dio_get_property( doc, "element_type" );
  if( !strcmp( element_type, "3DLinearTetrahedron" ) ) {
    global->t_elem = 4;
    t_elem = global->t_elem;
    size = global->nel * t_elem;
    global->nop = (int *) Calloc( sizeof(int), size );
    adv_dio_read_int32v( doc, 0, size, global->nop ); 
  } else {
    printf( "Element type not suppoerted.\n" );
    exit(1);
  }
  adv_dio_close( doc );  

  /******** Find Neighbour Elements *********/
  // Make_elel(global);
  // if (myid == 0) Save_elel(global, CASE, MODEL_ID);
  // MPI_Barrier(MPI_COMM_WORLD);
  Load_elel(global, CASE, MODEL_ID);
  Rfprintf(stderr, "get_global_mesh Done!\n");   
 }


void Make_elel(GlobalMesh *global){
  Rfprintf(stderr, "  Make_elel...\n");   
  int i, j, k, inop[4], t_elem = global->t_elem;
  const int None = -1,  Init = -2, diff = -1;
  int *count1, *count2, *count3, *count4, p0, p1, p2 , p3, *face1, *face2, *face3, *face4, *start, *end, size, done, sendcounts;
  int *recvcounts, *displs, *rbuf;

  int nproc = get_num_proc() ;
  int myid = get_myid();
  int np = size = global->nnodes;
  int **temp = (int **) Calloc( sizeof(int*), size );
  short int *ti = (short int *) Calloc( sizeof(short int), size ); // The length of temp[i]
  short int *CUR_nnd = (short int *) Calloc( sizeof(short int), size );// The length of allocated memory for npel
  
  /* Init */
  for (i=0; i< size;i++) {
    CUR_nnd[i] = MAX_NND;
    temp[i] = (int *) Calloc( sizeof(int), MAX_NND );
    for (j=0;j<MAX_NND;j++) temp[i][j] = None;
    ti[i] = 0;
  }
        
  global->elel =(int *)Calloc( sizeof(int), global->nel*t_elem );
  for (i=0;i<global->nel;i++) {    
    for (j=0; j<t_elem; j++)   global->elel[i*t_elem + j] = Init;
  }    
  
  /* Find element that contain this node */ 
  for (i=0; i< global->nel;i++) {       
    for (j=0; j<t_elem; j++) inop[j] = global->nop[i*t_elem+j];
    for (j=0; j<t_elem; j++){
      if (ti[inop[j]] >= CUR_nnd[inop[j]]) {//Not enough memory
        temp[inop[j]] = (int *) Realloc( temp[inop[j]], sizeof(int), CUR_nnd[inop[j]], CUR_nnd[inop[j]] + INC_NND );
        for (k=0;k<INC_NND;k++) temp[inop[j]][CUR_nnd[inop[j]]+k] = None;//Init
        CUR_nnd[inop[j]] += INC_NND; 
        Dprintf("Point[%d] MAX_NND increased to %d!\n",inop[j], CUR_nnd[inop[j]]);
      }
      temp[inop[j]][(ti[inop[j]])++] = i;
      
    }//j:t_elem
}//i:nel	 
  
/* Set the band for parallel computation */ 
int surface=3;
start = (int *) Calloc( sizeof(int), nproc );
end   = (int *) Calloc( sizeof(int), nproc );
Splitter(global->nel, nproc, start, end );
  
/* Parallell computation */ 
for (i=start[myid]; i< end[myid];i++) {
  
    /* Calculate nodes */          
    p0 = global->nop[i*t_elem];
    p1 = global->nop[i*t_elem+1];
    p2 = global->nop[i*t_elem+2];
    p3 = global->nop[i*t_elem+3];  
      
    /* Init */
    face1 = (int *) Calloc( sizeof(int), ti[p1]+ti[p2]+ti[p3] );
    face2 = (int *) Calloc( sizeof(int), ti[p0]+ti[p2]+ti[p3] );
    face3 = (int *) Calloc( sizeof(int), ti[p0]+ti[p1]+ti[p3] );
    face4 = (int *) Calloc( sizeof(int), ti[p0]+ti[p1]+ti[p2] );
      
    count1 = (int *) Calloc( sizeof(int), ti[p1]+ti[p2]+ti[p3] );
    count2 = (int *) Calloc( sizeof(int), ti[p0]+ti[p2]+ti[p3] );
    count3 = (int *) Calloc( sizeof(int), ti[p0]+ti[p1]+ti[p3] );
    count4 = (int *) Calloc( sizeof(int), ti[p0]+ti[p1]+ti[p2] );  
            
    /* Calculate faces */    
    //face 1 (1|2|3)
    for(j=0;j<ti[p1];j++) face1[j] = temp[p1][j];   
    for(j=0;j<ti[p2];j++) face1[j + ti[p1]] = temp[p2][j]; 
    for(j=0;j<ti[p3];j++) face1[j + ti[p1] + ti[p2]] = temp[p3][j];        
                
    //face 2 (0|2|3)
    for(j=0;j<ti[p0];j++) face2[j] = temp[p0][j];   
    for(j=0;j<ti[p2];j++) face2[j + ti[p0]] = temp[p2][j]; 
    for(j=0;j<ti[p3];j++) face2[j + ti[p0] + ti[p2]] = temp[p3][j]; 
      
    //face 3 (0|1|3)
    for(j=0;j<ti[p0];j++) face3[j] = temp[p0][j];   
    for(j=0;j<ti[p1];j++) face3[j + ti[p0]] = temp[p1][j]; 
    for(j=0;j<ti[p3];j++) face3[j + ti[p0] + ti[p1]] = temp[p3][j]; 
      
    //face 4 (0|1|2)
    for(j=0;j<ti[p0];j++) face4[j] = temp[p0][j];   
    for(j=0;j<ti[p1];j++) face4[j + ti[p0]] = temp[p1][j]; 
    for(j=0;j<ti[p2];j++) face4[j + ti[p0] + ti[p1]] = temp[p2][j]; 

//Count for point 1
  done = 0; 
  for (j=0;j<ti[p1]+ti[p2]+ti[p3];j++){  
    count1[j] = 1;
    for (k= j+1; k<ti[p1]+ti[p2]+ti[p3]; k++) {
      if (face1[j] != diff && face1[j] == face1[k]){
        count1[j]++;
        face1[k] = diff;
      }
    }
  }        
  for (j=0;j<ti[p1]+ti[p2]+ti[p3];j++){ 
    if ( face1[j]!=i && count1[j] == surface ) {
      //printf("FACE1 %d %d\n",face1[j],i);
      if (done == 1) printf("More than one neighbour? el:%d p:1\n", i);
      global->elel[(i-start[myid])*t_elem] = face1[j]; 
      done = 1;
      }
    }
  if (!done) global->elel[(i-start[myid])*t_elem] = None;

//Count for point 2
done = 0;
for (j=0;j<ti[p0]+ti[p2]+ti[p3];j++){  
  count2[j] = 1;  	  
  for (k= j+1; k<ti[p0]+ti[p2]+ti[p3]; k++) {
    if (face2[j] != diff && face2[j] == face2[k]){
      count2[j]++;
      face2[k] = diff;
      }
    }
}    
for (j=0;j<ti[p0]+ti[p2]+ti[p3];j++){ 
if ( face2[j]!=i && count2[j] == surface ) {
  //printf("FACE2 %d %d\n",face2[j],i);
  if (done == 1) printf("More than one neighbour?el:%d p:2\n",i);
  global->elel[ (i-start[myid])*t_elem + 1] = face2[j]; 
  done = 1;
  }
}
if (!done) global->elel[ (i-start[myid])*t_elem + 1] = None;

//Count for point 3
done = 0;
for (j=0;j<ti[p0]+ti[p1]+ti[p3];j++){  
  count3[j] = 1; 
  for (k= j+1; k<ti[p0]+ti[p1]+ti[p3]; k++) {
    if (face3[j] != diff && face3[j] == face3[k]){
      count3[j]++;
      face3[k] = diff;
      }
  }
}    
for (j=0;j<ti[p0]+ti[p1]+ti[p3];j++){ 
if ( face3[j]!=i && count3[j] == surface ) {
  //printf("FACE3 %d %d\n",face3[j],i);
  if (done == 1) printf("More than one neighbour?el:%d p:3\n",i);
  global->elel[ (i-start[myid])*t_elem + 2] = face3[j]; 
  done = 1;
  }
}
if (!done) global->elel[ (i-start[myid])*t_elem + 2] = None;   

//Count for point 4
done = 0;
for (j=0;j<ti[p0]+ti[p1]+ti[p2];j++){ 
  count4[j] = 1; 
  for (k= j+1; k<ti[p0]+ti[p1]+ti[p2]; k++) {
    if (face4[j] != diff && face4[j] == face4[k]){
      count4[j]++;
      face4[k] = diff;
      }
  }
} 
for (j=0;j<ti[p0]+ti[p1]+ti[p2];j++){ 
  if ( face4[j]!=i && count4[j] == surface ) {
  //printf("FACE4 %d %d\n",face4[j],i);
    if (done == 1) printf("More than one neighbour?el:%d p:4\n",i);
    global->elel[ (i-start[myid])*t_elem + 3] = face4[j]; 
    done = 1;
  }
}
if (!done) global->elel[ (i-start[myid])*t_elem + 3] = None; 

  Free( sizeof(int)*(ti[p1]+ti[p2]+ti[p3]),face1 );
  Free( sizeof(int)*(ti[p0]+ti[p2]+ti[p3]),face2 );
  Free( sizeof(int)*(ti[p0]+ti[p1]+ti[p3]),face3 );
  Free( sizeof(int)*(ti[p0]+ti[p1]+ti[p2]),face4 );
  
  Free( sizeof(int)*(ti[p1]+ti[p2]+ti[p3]),count1 );
  Free( sizeof(int)*(ti[p0]+ti[p2]+ti[p3]),count2 );
  Free( sizeof(int)*(ti[p0]+ti[p1]+ti[p3]),count3 );
  Free( sizeof(int)*(ti[p0]+ti[p1]+ti[p2]),count4 );
          
}//End of my job

   Free(sizeof(short int)*(size), ti);
   for (i=0; i< size;i++) Free(sizeof(int)*CUR_nnd[i], temp[i]); 
   Free(sizeof(short int)*(size), CUR_nnd);
   Free(sizeof(int*)*(size),temp);
 
   /* Init */
   size = global->nel * t_elem;
   recvcounts = (int *) Calloc( sizeof(int), nproc );
   displs = (int *) Calloc( sizeof(int), nproc );
   rbuf = (int *)Calloc( sizeof(int), size );
   
   /* detach big data pakage */
   int iend[nproc], istart[nproc], *send;

   for (i=0; i< end[myid] - start[myid]; i += MPI_Max){

   	 for (j=0; j<nproc; j++){
        istart[j] = i ;
        if ((end[j] - start[j] - istart[j]) >= MPI_Max )  iend[j] = istart[j] + MPI_Max;
        else iend[j] = istart[j] + (end[j] - start[j] - istart[j]);
        recvcounts[j] = (iend[j]-istart[j]) * t_elem;
        displs[j] = (start[j] + istart[j]) * t_elem;
     }

     send = &(global->elel[istart[myid] * t_elem]);
     sendcounts = (iend[myid] - istart[myid]) * t_elem; 
     MPI_Allgatherv(send, sendcounts, MPI_INT, rbuf, recvcounts, displs, MPI_INT, MPI_COMM_WORLD); 
   }
    
    Free(sizeof(int) * nproc, start);
    Free(sizeof(int) * nproc, end);    
    Free(sizeof(int) * nproc, recvcounts);  
    Free(sizeof(int) * nproc, displs);  
    Free(sizeof(int)*size, global->elel);
    global->elel = rbuf;
    rbuf = NULL;
}


  void Save_elel(GlobalMesh *global, char CASE[], char MODEL_ID[]){

    Rfprintf(stderr, "  Save_elel...\n");
    char  fname[256];
    sprintf(fname, "/public/home/graduate/sFlow-LUO2/Neural_Network/data/unstructed_%s/%s_%s/adv/elel.bin", CASE, CASE, MODEL_ID);
    FILE *fp = fopen(fname, "wb");
    if(fp == NULL) {
      printf("Error opening file\n");
    }
    fwrite(global->elel, sizeof(int), global->nel * 4, fp);
  }


  void Load_elel(GlobalMesh *global, char CASE[], char MODEL_ID[]){

    Rfprintf(stderr, "  Load_elel...\n");

    char  fname[256];
    sprintf(fname, "/public/home/graduate/sFlow-LUO2/Neural_Network/data/unstructed_%s/%s_%s/adv/elel.bin", CASE, CASE, MODEL_ID);
    global->elel =(int *)Calloc( sizeof(int), global->nel*4);
    FILE *fp = fopen(fname, "rb");
    fread(global->elel, sizeof(int), global->nel * 4, fp);
  }

 void Splitter(int range, int nproc, int *start, int *end ){
  
  int i, band;

  /* Divide the range */
  band = (range)/nproc;
  if ((range)-(nproc * band)){//remainder
    for (i = 0; i <(range)-(nproc * band); i++){
        start[i] = i * (band+1);
        end[i]   = start[i] + band + 1;
       }
    for (i = (range)-(nproc * band); i < nproc; i++){
        start[i] = end[i-1];
        end[i]   = start[i] + band;
      }
  }
  else{
    for (i = 0; i < nproc; i++){
       start[i] = i*band;
       end[i]   = start[i] + band;
    }  
  }
  
}