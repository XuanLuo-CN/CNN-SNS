#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include "Adv/AdvDocument.h"
#include "Adv/AdvUtil.h"
#include "hddm_types.h"
#include "mem.h"
#include "defs.h"
#include "comm_mpi.h"

void get_global_mesh (GlobalMesh *global, char CASE[], char MODEL_ID[]);
void Make_elel(GlobalMesh *global);
void Save_elel(GlobalMesh *global, char CASE[], char MODEL_ID[]);
void Load_elel(GlobalMesh *global, char CASE[], char MODEL_ID[]);
void Splitter(int range, int nproc, int *start, int *end);