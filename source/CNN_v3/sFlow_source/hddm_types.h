/***********************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
************************************************************/
#include "comm_mpi.h"
// #include <torch/torch.h>

#ifndef __HDDM_TYPES_H
#define __HDDM_TYPES_H

/* Switches for input data */
typedef struct
{
  char *data_dir;
  char *subdir;
  char *filename;

  int use_ns_resin;
  char *ns_resin_subdir;
  char *ns_resin_filename;
  int ns_resin_filename_step;
  int para;
} InputSW;

/* Switches for output data */
typedef struct
{
  char *data_dir;
  char *subdir;
  char *filename;

  int nsout_interval;
  int nsout_last;
  int para;
} OutputSW;

/* Global options */
typedef struct
{
  int nonsteady; /* set 0 for Statinary, 1 for Non-Stationary */
  int fem_sw;
  int ch_par;
  int use_tet10_5;
  int maxmem;
  int sw_kpmat;
} OptSolv;

/* Switches for CG method */
typedef struct
{
  double tol;
  int max_loop;
  int resout_last;
  int use_resin;
  int sw_precon;
  char *data_dir;
  char *resin_subdir;
  char *resin_filename;
  char *resout_subdir;
  char *resout_filename;
  int use_bddcmat;
} CGOpts;

typedef struct
{
  double dt;
  double fai;
  double init;
  int max_loop;
  double ns_tol;
  double totaltime;
} NSOpts;

/*+ Inner boundary condition +*/
typedef struct
{
  int aspart;      /* parent processor number         */
  int mypart_ibid; /* inner-boundary local  number    */
  int node;        /* node number in each domain      */
  int co;          /*+ coodinate of this dof (0=x, 1=y, 2=z, 3=p) +*/
} Inbc;

/*+ Share node +*/
typedef struct
{
  int aspart;      /*+ This node is assigned to part "aspart" +*/
  int mypart_snid; /*+ Share node ID in this part   +*/
  int node;        /*+ node ID in the subdomain +*/

  int asgdom;      /*+  Global Domid of asdom  +*/
  int asdom;       /*+  This node is assigned to sub domain "asdom" +*/
  int n_share_dom; /* How many domains share this node including this node */
  int *share_gdom; /*+ List of Global Domid sharing this node (n_share_dom -1 )  +*/

} Sharenode;

/*+ boundary condition  +*/
typedef struct
{
  int node;   /* node number                      */
  int co;     /* coordinate  (0=x, 1=y, 2=z, 3=p) */
  double val; /* value                            */
} Bcond;

typedef struct
{
  double nu;
  double kappa;
  double beta;
  double Tr;
  double g[3];
  double del_time;
} Material;

/*+ Struct of Skyline matrix  +*/
typedef struct
{

  int dim;   /*+ Dimension +*/
  int slen;  /*+ length of 1d array to store the matrix +*/
  int *nsum; /*+ the skyline index array  +*/
  int *ii;
  double *d; /*+ to store the matrix elements  +*/

} AsymSkylineMatrix; /*shape is sym, but values are not*/

/*+ Struct of Asymetric Skyline matrix  +*/
typedef struct
{

  int partid;
  int domid;
  int dim;
  int *bdinfo;
  AsymSkylineMatrix gk;
  AsymSkylineMatrix gk_inv;
  double *force; /* force */

  /*addid after*/
  int ntemp;
  int ninbd;

} HDDM_AsymSkylineMat;

typedef struct
{

  int dim;   /*+ Dimension +*/
  int slen;  /*+ length of 1d array to store the matrix +*/
  int *nsum; /*+ the skyline index array  +*/
  int *minind;
  double *d;     /*+ to store the matrix elements  +*/
} SkylineMatrix; /* symmetric matirx */

typedef struct
{

  int partid;
  int domid;
  int dim;
  int ntemp;
  int nflux;
  int ninbd;
  int *bdinfo;
  int state_gk;
  int state_gk_inv;
  SkylineMatrix gk;
  SkylineMatrix gk_inv;
  double *force;

} HDDM_SkylineMat;

typedef struct
{
  int *element; /* ElementIndex_SubdomainToGlobal */
  int n_element;
} DomToGlobal;

typedef struct
{
  int n_domain;
  DomToGlobal *domain;
} PartToGlobal;

/*+ Global Mesh +*/
typedef struct
{
  int need_coarse_matrix; /* switch of making the coarse matrix */
  int sw_ns;              /* state of the non-stationary loop*/
  int nsloop;             /* non-stationary loop             */
  int nel;                /* number of elements              */
  int nnodes;             /* number of nodes                 */
  int *elel;              /* neighbour elements              */
  double *crd;            /* coordinate                      */
  int *nop;               /* nodal point                     */
  int **el2dom;           /* map the global element to local */
  int t_elem;
  int sum_ntemp;
  int *ia; /* row index for CRS */
  int *ja; /* col index for CRS */
  double *T_cur;
  double *T_tmp;
  Bcond *temp;
  PartToGlobal *part;

  // *********** //
  int *el_belong;
  int *el_center;

} GlobalMesh;

/*+ SubDomain Mesh +*/
typedef struct
{
  int domid;
  int partid;
  int nel;     /* number of elements              */
  int nodes;   /* number of nodes                 */
  int ntemp;   /*           b.c. temp             */
  int nflux;   /*           b.c. flux             */
  int ninbd;   /*           inner-domain b.c.     */
  int ifn_dim; /* dim. of inbc                    */
  int nsharenode;
  int nd_elm;
  int nd_elm2;
  int *prevlist;
  double *jacob;
  int *nop; /* nodal point dom->part */
  int *ndindex;
  Inbc *inbd;
  Bcond *bctemp;
  Bcond *bcflux;
  Sharenode *sharenode;

  int gdomid; /*+ Global Subdomain ID +*/

  int node_dim; /* not dimenosion of node!! it is used some paterns*/

  char elm_type[128];

  double *temp;
  double *oldtmp;
  double *force;
  double *crd;

  /* material */
  int have_matid;
  int *matid;
  int nmat;
  Material *mat;

  /* LHS */
  SkylineMatrix gk;
  SkylineMatrix gk_inv;

} DomMesh;

/* Information about interpart dofs,
   x: this part (x), y: the other part */
typedef struct
{
  int n_mofree;  /* Number of midfree or outfree inbc's,
                    which is shared between the parts x and y
                    and also assigned to  x or y  */
  int *mof_ibid; /* The above inbc ID's ( the ID is for this part) */
} OPinfo;        /* Information of another part  known by this part */

typedef struct
{
  int n_moshare; /*+ num of share nodes which assigned to me or
                        to the other part  */
  int *mo_snnid; /*+ node ID of share ndoes */
} OPSinfo;       /* Information for nodes shareing with other part */

/*+ Part Mesh  +*/
typedef struct
{
  int partid;    /*+ part ID  +*/
  int n_part;    /*+ num part  +*/
  int n_domain;  /* Number of in  domains  */
  int n_outdom;  /* Number of out domains  */
  int t_nodes;   /* Total number of nodes  */
  int t_infree;  /* Total inner   freedom  */
  int t_outfree; /* Total outer   freedom  */
  int t_midfree; /* Total middler freedom  */
  int sum_nel;   /* Sumation of nel of all domains   */
  int sum_nodes; /* Sumation of nodes of all domains */
  int sum_nel_orig, sum_nodes_orig;
  int sum_ntemp;   /* Sumation of the number of
                     templacement B.C. of all domains */
  int sum_nflux;   /* Sumation of the number
                     of flux B.C. of all domains      */
  int sum_ninbd;   /* Sumation of the number of
                     inner boundary nodes             */
  int *Index_node; /*+ part to global  +*/

  int have_matid;
  int nmat;
  Material *mat;

  OPinfo *op;    /*+ inter part inner bc info +*/
  OPSinfo *opsn; /*+ inter part share nodes info +*/

  int t_insnode;  /*+ num of share nodes assigned to this part +*/
  int t_outsnode; /*+ num of share nodes assigned to other parts +*/

  DomMesh *dom; /*+ Sub domain meshes  +*/

  int global_t_domains;
  int *off_gdom; /*+ GlobalDomID = domid (in ipart) + off_gdom[ipart]  +*/ /*+*/

  /*- Send CC -*/
  int **isbuf, *si, *CC_si, *pos;
  COM_Request *Reqst_0, **Reqst_1;
  double **dsbuf, **CC_string;

} PartMesh;

#endif /* __HDDM_TYPES_H */
