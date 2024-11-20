/***********************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
************************************************************/

#ifndef __DEFS_H
#define __DEFS_H

/*+  Extention of ADV files +*/
#define ADVFILE_EXT ".adv" 

#define ADVSFLOW_NAME "ADVENTURE_SFlow"
#define ADVSFLOW_VERSION "0.8 (beta)"
#define MODULE_NAME "advsflow"
#define MPI_Max 4096

#define DATA_DIR                 "indata"
#define INDATA_SUBDIR            "model"
#define INDATA_FILENAME          "advhddm_in"
#define OUTDATA_SUBDIR           "result"
#define OUTDATA_FILENAME         "advhddm_out"
#define RESIN_SUBDIR             "cg-res"
#define RESIN_FILENAME           "advhddm_cgres"
#define RESOUT_SUBDIR            "cg-res"
#define RESOUT_FILENAME          "advhddm_cgres"
#define NSIN_SUBDIR              "result"
#define NSIN_FILENAME            "advhddm_out"
#define NSRESIN_SUBDIR           "result"
#define NSRESIN_FILENAME         "advhddm_out"
	
	
/*+ Test Mode: solve the lid driven problem and thermal problem seperately +*/
//#define TEST_MODE  //Make sure that COUPLING_ANALYSIS is used.
	
/*+ MCC: Mass Conservation2 +*/
//#define MCC
	
/*+ The type of the Lefe Hand Side matrix +*/
//#define COUPLING_ANALYSIS


/*+ The type of the Lefe Hand Side matrix +*/
//#define NO_TIME_STAB     //Do NOT use only time stab. (del_T/2)
//#define TAU_CO 

/*+ The type of the Lefe Hand Side matrix +*/
#define SAME_LHS

/*+ The initional value of CG loop +*/	
//#define CG_FROM_ZERO //Start form 0

/* Pressue and Velocity Gradient */
//#define PV_GRAD


/*+ If absolute (not relative) norm is less than 
  this at the first CG step, we denote it's already converged  +*/
#define CG_NEARLY_ZERO     1.0e-60

/*+  Limitation of memory/PE in bytes +*/
#define MEMLIMIT 1200*1024*1024

/*+  Searching Sucess Criteria +*/
#define SRH_EPS 1.0e-6

#define SWKP_NOKEEP     0
#define SWKP_MAKE       1
#define SWKP_USEKEPT    2

#define CG_PRECON_NONE      0
#define CG_PRECON_DIAG      1
#define CG_PRECON_NN        2
#define CG_PRECON_BDD       3
#define CG_PRECON_BDD_DIAG  4

#define SWCG_FIRST  1
#define SWCG_ITER   2
#define SWCG_LAST   3
#define SWCG_END    4
#define SWCG_REINIT 5

#define NS_FIRST  1
#define NS_ITER   2
#define NS_LAST   3
	
/*+ Default Maximun CG loops  +*/
#define MAX_CG_LOOP 10000

/*+ Default CG tolerance +*/ 
#define EPS 1.0e-6
#define NSEPS 1.0e-4
#define MAXTOTALTIME 10000

/*+ Default Maximum neighbour node  +*/
#define MAX_NND 30
#define INC_NND 10

#define INITIAL 0.0
#define DELTA_T 1.0
#define FAI 1.0
#define MAX_NS_LOOP 10

#define NO_NEED  0
#define NEED     1



/*max norm or eucrid norm*/
#define MAX_NORM 1
#define EUC_NORM 2


/*+ �������莞�Ɏg�������c���m�����̐ݒ���@ +*/
   /* Original: ��ԍŏ��̃X�e�b�v�Ō��߂��̂������Ǝg�� */
//#define  REF_CGNORM_RESET_WAY    0                    
   /* New: �O�ɐݒ肵�����̂��傫�ȏ����c���̏ꍇ�A���݂̂��̂ɍēx�ݒ肷��B */
   	   
   	     /* flag =0 ... �ݒ�ςȂ���Ƃ̂܂� */
         /* flag =1 ... �K���㏑������ */
         /* flag =2 ... �ݒ肵�ĂȂ���ΐݒ肷��B�ݒ�ςł��ݒ�l���
                        �傫����ΐݒ肵���� */
   	   
   	   
#define  REF_CGNORM_RESET_WAY    2



#define INVMATSTATE_NONE            0
#define INVMATSTATE_FIX_BCDISP      1
#define INVMATSTATE_FIX_INBC        2
#define INVMATSTATE_NOFIX_GINV      3
                   /*for domain solver function */




#endif /* __DEFS__H  */


















