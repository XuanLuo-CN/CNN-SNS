#include <stdio.h>
#include <stdlib.h>
#include "sFlow_source/hddm_types.h"
#include "sFlow_source/get_global_mesh.h"
#include "sFlow_source/comm_mpi.h"

//Cal average of parr time for Re
char* CASE; 
char* MODEL_ID;

int main(int argc, char* argv[])
{
    // ***********Init*********//
    CASE = argv[16];
    MODEL_ID = argv[1];
    GlobalMesh *global ;

    COM_Init(&argc, &argv) ;
    Rfprintf(stderr, "Init data ...\n");   
    global = (GlobalMesh  *)calloc(sizeof(GlobalMesh), 1);
    get_global_mesh(global, CASE, MODEL_ID);
    Rfprintf(stderr, "nel: %d   nnodes: %d\n", global->nel, global->nnodes);
    
    NN(global, argv);
    // if(COM_ID.myprocid == 0){
    //     NN(global, argv);
    // }
    MPI_Barrier(MPI_COMM_WORLD);
    COM_Finalize();
    exit(0);
}